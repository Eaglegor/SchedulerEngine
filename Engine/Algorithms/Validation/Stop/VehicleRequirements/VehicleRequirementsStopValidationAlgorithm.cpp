#include "VehicleRequirementsStopValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/RunBoundaryStop.h>
#include <Engine/SceneManager/ConstStopVisitor.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/Utils/CallableVisitorProxy.h>

class RequirementsChecker : public Scheduler::ConstStopVisitor
{
public:
	using ReturnType = bool;

	RequirementsChecker()
		: is_valid(true)
	{
	}

	virtual void dispatch(const Scheduler::WorkStop* work_stop) override
	{
		is_valid = true;
		const Scheduler::Operation* operation = work_stop->getOperation();
		const Scheduler::Vehicle* vehicle = work_stop->getRun()->getVehicle();
		if (vehicle != nullptr && operation->constraints().vehicleAttributesRequirements().isSet())
		{
			for(const Scheduler::Attribute* requirement : operation->constraints().vehicleAttributesRequirements().get())
			{
				is_valid = is_valid && util::contains_key(vehicle->getAttributes(), requirement);
				if (!is_valid) break;
			}
		}
	}

	virtual void dispatch(const Scheduler::RunBoundaryStop* run_boundary_stop) override
	{
		is_valid = true;
		for (const Scheduler::Operation* operation : run_boundary_stop->getOperations())
		{
			const Scheduler::Vehicle* vehicle = run_boundary_stop->getRun()->getVehicle();
			if (vehicle != nullptr && operation->constraints().vehicleAttributesRequirements().isSet())
			{
				for (const Scheduler::Attribute* requirement : operation->constraints().vehicleAttributesRequirements().get())
				{
					is_valid = is_valid && util::contains_key(vehicle->getAttributes(), requirement);
					if (!is_valid) break;
				}
			}
			if (!is_valid) break;
		}
	}

	ReturnType getRetVal() const
	{
		return is_valid;
	}

private:
	bool is_valid;
};

bool Scheduler::VehicleRequirementsStopValidationAlgorithm::isValid(const Stop * stop) const
{
	return CallableVisitorProxy().call<RequirementsChecker>(stop);
}
