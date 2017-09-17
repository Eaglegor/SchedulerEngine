#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Stop;
	class Operation;
	class Attribute;

	/**
	 * @ingroup constraints
	 * 
	 * @brief Violation of the required performer skill order constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class PerformerSkillsRequirementsViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param stop Stop violating the constraint
		 * @param operation Operation constraint of which is violated
		 * @param missing_skill The missing required skill
		 */
		PerformerSkillsRequirementsViolation(const Stop& stop, const Operation& operation, const Attribute& missing_skill)
		    : stop(stop),
		      operation(operation),
		      missing_skill(missing_skill)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		PerformerSkillsRequirementsViolation(const PerformerSkillsRequirementsViolation& rhs)
		    : stop(rhs.stop),
		      operation(rhs.operation),
		      missing_skill(rhs.missing_skill)
		{
		}

		/**
		 * @brief Returns the stop violating the constraint
		 * 
		 * @return Stop violating the constraint
		 */
		const Stop& getStop( ) const
		{
			return stop;
		}

		/**
		 * @brief Returns the operation constraint of which was violated
		 * 
		 * @return The operation constraint of which was violated
		 */
		const Operation& getOperation( ) const
		{
			return operation;
		}

		/**
		 * @brief Returns the missing required skill
		 * 
		 * @return The missing required skill
		 */
		const Attribute& getMissingSkill( ) const
		{
			return missing_skill;
		}

		/**
		 * @brief Returns the violation type
		 * 
		 * @return Violation type
		 */
		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::PERFORMER_SKILLS_REQUIREMENTS_VIOLATION;
		}

	private:
		const Stop& stop;
		const Operation& operation;
		const Attribute& missing_skill;
	};
}