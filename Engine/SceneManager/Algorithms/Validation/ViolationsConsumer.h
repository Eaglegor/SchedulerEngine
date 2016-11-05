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
#include "Violations/Schedule/ScheduleShiftEndLocationViolation.h"
#include "Violations/Schedule/ScheduleShiftStartLocationViolation.h"
#include "Violations/Schedule/ScheduleWorkingTimeLimitViolation.h"

#include "Violations/Stop/OperationTimeWindowsViolation.h"
#include "Violations/Stop/PerformerSkillsRequirementsViolation.h"
#include "Violations/Stop/VehicleAttributesRequirementsViolation.h"

#include "ValidationContinuancePolicy.h"

namespace Scheduler
{
	class ViolationsConsumer
	{
	public:
		virtual ~ViolationsConsumer(){}
		
		virtual ValidationContinuancePolicy genericConsumeViolation(const ConstraintViolation& violation){return ValidationContinuancePolicy::CONTINUE;}
		
		virtual ValidationContinuancePolicy consumeViolation(const RunDistanceLimitViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const RunDrivingTimeLimitViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const RunWorkingTimeLimitViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const VehicleAvailabilityWindowsViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const VehicleCapacityViolation& violation){return genericConsumeViolation(violation);}
		
		virtual ValidationContinuancePolicy consumeViolation(const PerformerAvailabilityWindowsViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleDistanceLimitViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleDrivingTimeLimitViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleShiftEndLocationViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleShiftStartLocationViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const ScheduleWorkingTimeLimitViolation& violation){return genericConsumeViolation(violation);}
		
		virtual ValidationContinuancePolicy consumeViolation(const OperationTimeWindowsViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const PerformerSkillsRequirementsViolation& violation){return genericConsumeViolation(violation);}
		virtual ValidationContinuancePolicy consumeViolation(const VehicleAttributesRequirementsViolation& violation){return genericConsumeViolation(violation);}
		
		virtual bool supportsViolationType(ConstraintViolationType type) {return true;}
	};
}