#pragma once

#include "ViolationsConsumer.h"

namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief Simple validator class that detects if the object has any violations
	 */
	class Validator : public ViolationsConsumer
	{
	public:
		enum class ValidationResult
		{
			VALID,
			INVALID
		};

		virtual ValidationContinuancePolicy genericConsumeViolation(const ConstraintViolation& violation) override
		{
			result = Validator::ValidationResult::INVALID;
			return ValidationContinuancePolicy::INTERRUPT;
		}

		/**
		 * @brief Returns the validation result
		 * 
		 * @return ValidationResult::VALID if there were no violations reported,
		 * ValidationResult::INVALID otherwise
		 */
		ValidationResult getValidationResult( ) const
		{
			return result;
		}

	private:
		ValidationResult result = ValidationResult::VALID;
	};
}