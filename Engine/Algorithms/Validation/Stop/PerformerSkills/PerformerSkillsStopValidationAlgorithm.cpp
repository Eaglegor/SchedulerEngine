#include "PerformerSkillsStopValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/RunBoundaryStop.h>
#include <Engine/SceneManager/ConstStopVisitor.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/Utils/CallableVisitorProxy.h>

class SkillsChecker : public Scheduler::ConstStopVisitor
{
public:
	using ReturnType = bool;

	SkillsChecker()
		: is_valid(true)
	{
	}

	virtual void dispatch(const Scheduler::WorkStop* work_stop) override
	{
		is_valid = true;
		const Scheduler::Order* order = work_stop->getOperation()->getOrder();
		const Scheduler::Performer* performer = work_stop->getRun()->getSchedule()->getPerformer();
		if (order != nullptr)
		{
			for(const Scheduler::Attribute* requirement : order->getPerformerSkillsRequirements())
			{
				is_valid = is_valid && std::contains_key(performer->getSkills(), requirement);
				if (!is_valid) break;
			}
		}
	}

	virtual void dispatch(const Scheduler::RunBoundaryStop* run_boundary_stop) override
	{
		is_valid = true;
		for (const Scheduler::Operation* operation : run_boundary_stop->getOperations())
		{
			const Scheduler::Order* order = operation->getOrder();
			const Scheduler::Performer* performer = run_boundary_stop->getRun()->getSchedule()->getPerformer();
			if (order != nullptr)
			{
				for (const Scheduler::Attribute* requirement : order->getPerformerSkillsRequirements())
				{
					is_valid = is_valid && std::contains_key(performer->getSkills(), requirement);
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

bool Scheduler::PerformerSkillsStopValidationAlgorithm::isValid(const Stop * stop) const
{
	return CallableVisitorProxy().call<SkillsChecker>(stop);
}
