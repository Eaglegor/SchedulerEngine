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

	virtual void dispatch(const Scheduler::WorkStop& work_stop) override
	{
		is_valid = true;
		const Scheduler::Operation& operaton = work_stop.getOperation();
		const Scheduler::Performer& performer = work_stop.getRun().getSchedule().getPerformer();
		if (operaton.constraints().performerSkillsRequirements().isSet())
		{
			for(const Scheduler::Performer::Skill& requirement : operaton.constraints().performerSkillsRequirements().get())
			{
				is_valid = is_valid && util::contains_key(performer.getSkills(), requirement);
				if (!is_valid) break;
			}
		}
	}

	virtual void dispatch(const Scheduler::RunBoundaryStop& run_boundary_stop) override
	{
		is_valid = true;
		for (const Scheduler::Operation& operation : run_boundary_stop.getOperations())
		{
			const Scheduler::Performer& performer = run_boundary_stop.getRun().getSchedule().getPerformer();
			if (operation.constraints().performerSkillsRequirements().isSet())
			{
				for (const Scheduler::Performer::Skill& requirement : operation.constraints().performerSkillsRequirements().get())
				{
					is_valid = is_valid && util::contains_key(performer.getSkills(), requirement);
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

bool Scheduler::PerformerSkillsStopValidationAlgorithm::isValid(const Stop& stop) const
{
	return CallableVisitorProxy().call<SkillsChecker>(stop);
}
