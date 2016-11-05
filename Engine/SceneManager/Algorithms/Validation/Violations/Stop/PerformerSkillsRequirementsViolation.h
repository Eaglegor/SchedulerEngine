#pragma once 

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Stop;
	class Operation;
	class Attribute;
	
	class PerformerSkillsRequirementsViolation : public ConstraintViolation
	{
	public:
		PerformerSkillsRequirementsViolation(const Stop& stop, const Operation& operation, const Attribute& missing_skill):
		stop(stop),
		operation(operation),
		missing_skill(missing_skill)
		{
		}
		
		PerformerSkillsRequirementsViolation(const PerformerSkillsRequirementsViolation &rhs):
		stop(rhs.stop),
		operation(rhs.operation),
		missing_skill(rhs.missing_skill)
		{
		}
		
		const Stop& getStop() const
		{
			return stop;
		}
		
		const Operation& getOperation() const
		{
			return operation;
		}
		
		const Attribute& getMissingSkill() const
		{
			return missing_skill;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::PERFORMER_SKILLS_REQUIREMENTS_VIOLATION;
		}
		
	private:
		const Stop& stop;
		const Operation& operation;
		const Attribute& missing_skill;
	};
}