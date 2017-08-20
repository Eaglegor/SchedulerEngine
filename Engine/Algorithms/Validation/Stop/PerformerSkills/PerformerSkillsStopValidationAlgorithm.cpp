#include "PerformerSkillsStopValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/DepotStop.h>
#include <Engine/SceneManager/ConstStopVisitor.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <Engine/SceneManager/DepotOperation.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>

namespace Scheduler
{
	class SkillsChecker : public ConstStopVisitor
	{
	public:
		SkillsChecker(ViolationsConsumer& violations_consumer):
		violations_consumer(violations_consumer)
		{
		}

		virtual void dispatch(const WorkStop& work_stop) override
		{
			const Operation& operation = work_stop.getOperation();
			const Performer& performer = work_stop.getRun().getSchedule().getPerformer();
			
			if (operation.constraints().performerSkillsRequirements().isSet())
			{
				for(const Performer::Skill& requirement : operation.constraints().performerSkillsRequirements().get())
				{
					if(!util::contains_key(performer.getSkills(), requirement)) 
					{
						auto continuation_policy = violations_consumer.consumeViolation(PerformerSkillsRequirementsViolation(work_stop, operation, requirement));
						if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
					}
				}
			}
		}

		virtual void dispatch(const DepotStop& run_boundary_stop) override
		{
			for (const Operation& operation : run_boundary_stop.getOperations())
			{
				const Performer& performer = run_boundary_stop.getRun().getSchedule().getPerformer();
				if (operation.constraints().performerSkillsRequirements().isSet())
				{
					for (const Performer::Skill& requirement : operation.constraints().performerSkillsRequirements().get())
					{
						if(!util::contains_key(performer.getSkills(), requirement))
						{
							auto continuation_policy = violations_consumer.consumeViolation(PerformerSkillsRequirementsViolation(run_boundary_stop, operation, requirement));
							if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
						}
					}
				}
			}
		}

	private:
		ViolationsConsumer& violations_consumer;
	};

	void PerformerSkillsStopValidationAlgorithm::validate(const Stop& stop, ViolationsConsumer& violations_consumer) const
	{
		if(!violations_consumer.supportsViolationType(ConstraintViolationType::PERFORMER_SKILLS_REQUIREMENTS_VIOLATION)) return;
		
		SkillsChecker checker(violations_consumer);
		stop.acceptVisitor(checker);
	}
}
