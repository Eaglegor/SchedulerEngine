#pragma once

#include <memory>
#include <Engine/Concepts/Distance.h>
#include <Engine/Concepts/Duration.h>

#include <SceneManager_export.h>
#include <Engine/Concepts/Site.h>
#include "../ConstraintHolder.h"

namespace Scheduler
{
	class SCENEMANAGER_EXPORT ScheduleConstraints
	{
		public:
			using ScheduleDistanceLimitConstraint = ConstraintHolder<Distance>;
			using RunDistanceLimitConstraint = ConstraintHolder<Distance>;

			using ScheduleDrivingTimeLimitConstraint = ConstraintHolder<Duration>;
			using RunDrivingTimeLimitConstraint = ConstraintHolder<Duration>;

			using ScheduleWorkingTimeLimitConstraint = ConstraintHolder<Duration>;
			using RunWorkingTimeLimitConstraint = ConstraintHolder<Duration>;

			using ScheduleShiftStartSiteConstraint = ConstraintHolder<Site>;
			using ScheduleShiftEndSiteConstraint = ConstraintHolder<Site>;

			const ScheduleDistanceLimitConstraint& scheduleDistanceLimit() const;
			ScheduleDistanceLimitConstraint& scheduleDistanceLimit();

			const RunDistanceLimitConstraint& runDistanceLimit() const;
			RunDistanceLimitConstraint& runDistanceLimit();

			const ScheduleDrivingTimeLimitConstraint& scheduleDrivingTimeLimit() const;
			ScheduleDrivingTimeLimitConstraint& scheduleDrivingTimeLimit();

			const RunDrivingTimeLimitConstraint& runDrivingTimeLimit() const;
			RunDrivingTimeLimitConstraint& runDrivingTimeLimit();

			const ScheduleWorkingTimeLimitConstraint& scheduleWorkingTimeLimit() const;
			ScheduleWorkingTimeLimitConstraint& scheduleWorkingTimeLimit();

			const RunWorkingTimeLimitConstraint& runWorkingTimeLimit() const;
			RunWorkingTimeLimitConstraint& runWorkingTimeLimit();

			const ScheduleShiftStartSiteConstraint& shiftStartSite() const;
			ScheduleShiftStartSiteConstraint& shiftStartSite();

			const ScheduleShiftEndSiteConstraint& shiftEndSite() const;
			ScheduleShiftEndSiteConstraint& shiftEndSite();

		private:
			ScheduleDistanceLimitConstraint schedule_distance_limit_constraint;
			RunDistanceLimitConstraint run_distance_limit_constraint;

			ScheduleDrivingTimeLimitConstraint schedule_driving_time_limit_constraint;
			RunDrivingTimeLimitConstraint run_driving_time_limit_constraint;

			ScheduleWorkingTimeLimitConstraint schedule_working_time_limit_constraint;
			RunWorkingTimeLimitConstraint run_working_time_limit_constraint;

			ScheduleShiftStartSiteConstraint schedule_shift_start_location_constraint;
			ScheduleShiftEndSiteConstraint schedule_shift_end_location_constraint;
	};
}
