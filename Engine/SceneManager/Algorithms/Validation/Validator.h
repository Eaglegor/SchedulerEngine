#pragma once

#include "ViolationsConsumer.h"

namespace Scheduler
{
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
		
		ValidationResult getValidationResult() const
		{
			return result;
		}
		
	private:
		ValidationResult result = ValidationResult::VALID;
	};
}