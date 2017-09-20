#pragma once

#include "Violations/ConstraintViolation.h"

#include "Violations/Run/RunDistanceLimitViolation.h"
#include "Violations/Run/RunDrivingTimeLimitViolation.h"
#include "Violations/Run/RunWorkingTimeLimitViolation.h"
#include "Violations/Run/VehicleAvailabilityWindowsViolation.h"
#include "Violations/Run/VehicleCapacityViolation.h"

#include "Violations/Schedule/PerformerAvailabilityWindowsViolation.h"
#include "Violations/Schedule/ScheduleDistanceLimitViolation.h"
#include "Violations/Schedule/ScheduleDrivingTimeLimitViolation.h"
#include "Violations/Schedule/ScheduleWorkingTimeLimitViolation.h"

#include "Violations/Stop/OperationTimeWindowsViolation.h"
#include "Violations/Stop/PerformerSkillsRequirementsViolation.h"
#include "Violations/Stop/VehicleAttributesRequirementsViolation.h"

#include "ValidationContinuancePolicy.h"

namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief Class used to process detected constraints violations
	 * 
	 * @sa @ref scene_validation
	 */
	class ViolationsConsumer
	{
	public:
		virtual ~ViolationsConsumer( )
		{
		}

		/**
		 * @brief Consumes violation not considering it's context
		 * 
		 * @param violation Violation to consume
		 * 
		 * @todo Extract into the GenericViolationsConsumer class
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy genericConsumeViolation(const ConstraintViolation& violation)
		{
			return ValidationContinuancePolicy::CONTINUE;
		}

		/**
		 * @brief Consumes the run distance limit violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const RunDistanceLimitViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the run driving time limit violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const RunDrivingTimeLimitViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the run working time limit violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const RunWorkingTimeLimitViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the vehicle availability windows violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const VehicleAvailabilityWindowsViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the vehicle capacity violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const VehicleCapacityViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the performer availability windows violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const PerformerAvailabilityWindowsViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the schedule distance limit violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleDistanceLimitViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the schedule driving time limit violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleDrivingTimeLimitViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the schedule working time limit violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleWorkingTimeLimitViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the operation time windows violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const OperationTimeWindowsViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the performer skills requirements violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const PerformerSkillsRequirementsViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Consumes the vehicle attributes requirements violation
		 * 
		 * @param violation Violation to consume
		 * 
		 * @return ValidationContinuancePolicy::CONTINUE, if validation algorithm should continue validation,
		 * ValidationContinuancePolicy::INTERRUPT otherwise
		 */
		virtual ValidationContinuancePolicy consumeViolation(const VehicleAttributesRequirementsViolation& violation)
		{
			return genericConsumeViolation(violation);
		}

		/**
		 * @brief Checks if the consumer supports processing of the specified violation type
		 * 
		 * @param type Violation type to check for
		 * 
		 * @return @c true, if consumer supports the violation, @c false otherwise
		 */
		virtual bool supportsViolationType(ConstraintViolationType type)
		{
			return true;
		}
	};
}