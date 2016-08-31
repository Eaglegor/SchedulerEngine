#pragma once

#include <memory>
#include <Engine/Concepts/Distance.h>
#include <Engine/Concepts/Duration.h>

#include <SceneManager_export.h>
#include <Engine/Concepts/Site.h>
#include "../ConstraintHolder.h"
#include <Engine/SceneManager/Location.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT ScheduleConstraints
	{
		public:
			class LocationConstraint
			{
			public:
				LocationConstraint(const Location& location):location(location){};
				LocationConstraint(const LocationConstraint &rhs):location(rhs.location){}
				
				operator const Location&() const
				{
					return location;
				}
				
				const Location& get() const
				{
					return location;
				}
				
				bool operator==(const LocationConstraint& rhs) const
				{
					return location == rhs.location;
				}
				
			private:
				const Location& location;
			};
			
			using ScheduleDistanceLimitConstraint = ConstraintHolder<Distance>;
			using RunDistanceLimitConstraint = ConstraintHolder<Distance>;

			using ScheduleDrivingTimeLimitConstraint = ConstraintHolder<Duration>;
			using RunDrivingTimeLimitConstraint = ConstraintHolder<Duration>;

			using ScheduleWorkingTimeLimitConstraint = ConstraintHolder<Duration>;
			using RunWorkingTimeLimitConstraint = ConstraintHolder<Duration>;

			using ScheduleShiftStartLocationConstraint = ConstraintHolder<LocationConstraint>;
			using ScheduleShiftEndLocationConstraint = ConstraintHolder<LocationConstraint>;

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

			const ScheduleShiftStartLocationConstraint& shiftStartLocation() const;
			ScheduleShiftStartLocationConstraint& shiftStartLocation();

			const ScheduleShiftEndLocationConstraint& shiftEndLocation() const;
			ScheduleShiftEndLocationConstraint& shiftEndLocation();

		private:
			ScheduleDistanceLimitConstraint schedule_distance_limit_constraint;
			RunDistanceLimitConstraint run_distance_limit_constraint;

			ScheduleDrivingTimeLimitConstraint schedule_driving_time_limit_constraint;
			RunDrivingTimeLimitConstraint run_driving_time_limit_constraint;

			ScheduleWorkingTimeLimitConstraint schedule_working_time_limit_constraint;
			RunWorkingTimeLimitConstraint run_working_time_limit_constraint;

			ScheduleShiftStartLocationConstraint schedule_shift_start_location_constraint;
			ScheduleShiftEndLocationConstraint schedule_shift_end_location_constraint;
	};
}
