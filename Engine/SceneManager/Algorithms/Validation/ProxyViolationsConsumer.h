#pragma once

#include "ValidationContinuancePolicy.h"
#include "ViolationsConsumer.h"
#include <assert.h>

namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief Utilitary class redirecting the violations to another consumer and storing the
	 * returned continuation policy
	 * 
	 * @details This utilitary class is used by the chain validation algorithms that have to
	 * know if they need to be interrupted after the last called nested validation algorith.
	 * 
	 * This class redirects the violation to the passed consumer and stores the continuation
	 * policy returned by it. This policy value may later be retrieved using the getter.
	 */
	class ProxyViolationsConsumer : public ViolationsConsumer
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param backend The violations consumer doing actual work
		 */
		ProxyViolationsConsumer(ViolationsConsumer& backend)
		    : backend(backend)
		{
		}

		virtual ValidationContinuancePolicy genericConsumeViolation(const ConstraintViolation& violation) override
		{
			assert(false); // We don't go there
			return ValidationContinuancePolicy::CONTINUE;
		}

		virtual ValidationContinuancePolicy consumeViolation(const VehicleAttributesRequirementsViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const PerformerSkillsRequirementsViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const OperationTimeWindowsViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const ScheduleWorkingTimeLimitViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const ScheduleDrivingTimeLimitViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const ScheduleDistanceLimitViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const PerformerAvailabilityWindowsViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const VehicleCapacityViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const VehicleAvailabilityWindowsViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const RunWorkingTimeLimitViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const RunDrivingTimeLimitViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual ValidationContinuancePolicy consumeViolation(const RunDistanceLimitViolation& violation) override
		{
			return policy = backend.consumeViolation(violation);
		}

		virtual bool supportsViolationType(ConstraintViolationType type)
		{
			return backend.supportsViolationType(type);
		}

		ValidationContinuancePolicy getCurrentContinuancePolicy( ) const
		{
			return policy;
		}

	private:
		ViolationsConsumer& backend;
		ValidationContinuancePolicy policy = ValidationContinuancePolicy::CONTINUE;
	};
}