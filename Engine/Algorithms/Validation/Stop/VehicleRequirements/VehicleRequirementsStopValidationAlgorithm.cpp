#include "VehicleRequirementsStopValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/RunBoundaryStop.h>
#include <Engine/SceneManager/ConstStopVisitor.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/Utils/Optional.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>

namespace Scheduler
{
	class RequirementsChecker : public ConstStopVisitor
	{
	public:
		RequirementsChecker(ViolationsConsumer& violations_consumer):
		violations_consumer(violations_consumer)
		{
		}

		virtual void dispatch(const WorkStop& work_stop) override
		{
			const Scheduler::Operation& operation = work_stop.getOperation();
			Scheduler::Optional<const Scheduler::Vehicle&> vehicle = work_stop.getRun().getVehicle();
			
			if (vehicle && operation.constraints().vehicleAttributesRequirements().isSet())
			{
				for(const Scheduler::Vehicle::Attribute& requirement : operation.constraints().vehicleAttributesRequirements().get())
				{
					if(!util::contains_key(vehicle->getAttributes(), requirement))
					{
						auto continuation_policy = violations_consumer.consumeViolation(VehicleAttributesRequirementsViolation(work_stop, operation, requirement));
						if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
					}
				}
			}
		}

		virtual void dispatch(const Scheduler::RunBoundaryStop& run_boundary_stop) override
		{
			for (const Scheduler::Operation& operation : run_boundary_stop.getOperations())
			{
				Scheduler::Optional<const Scheduler::Vehicle&> vehicle = run_boundary_stop.getRun().getVehicle();
				if (vehicle && operation.constraints().vehicleAttributesRequirements().isSet())
				{
					for (const Scheduler::Vehicle::Attribute& requirement : operation.constraints().vehicleAttributesRequirements().get())
					{
						if(!util::contains_key(vehicle->getAttributes(), requirement))
						{
							auto continuation_policy = violations_consumer.consumeViolation(VehicleAttributesRequirementsViolation(run_boundary_stop, operation, requirement));
							if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
						}
					}
				}
			}
		}

	private:
		ViolationsConsumer& violations_consumer;
	};

	void VehicleRequirementsStopValidationAlgorithm::validate(const Stop& stop, ViolationsConsumer& violations_consumer) const
	{
		if(!violations_consumer.supportsViolationType(ConstraintViolationType::VEHICLE_ATTRIBUTES_REQUIREMENTS_VIOLATION)) return;
		
		RequirementsChecker checker(violations_consumer);
		stop.acceptVisitor(checker);
	}
}