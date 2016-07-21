#pragma once

#include <memory>
#include <Engine/Concepts/Distance.h>
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class RunLimitationsConstraint;
	class ScheduleLimitationsConstraint;
	
	class ScheduleConstraints
	{
		public:
			enum class DistanceConstraint
			{
				RUN_DISTANCE_LIMIT,
				SCHEDULE_DISTANCE_LIMIT
			};

			enum class DurationConstraint
			{
				RUN_DRIVING_TIME,
				RUN_WORKING_TIME,
				SCHEDULE_DRIVING_TIME,
				SCHEDULE_WORKING_TIME
			};

			const Distance* scheduleDistanceLimit() const;
			const Duration* scheduleDrivingTimeLimit() const;
			const Duration* scheduleWorkingTimeLimit() const;

			const Distance* runDistanceLimit() const;
			const Duration* runDrivingTimeLimit() const;
			const Duration* runWorkingTimeLimit() const;
		
			void addConstraint(const DistanceConstraint& type, const Distance& distance);
			void addConstraint(const DurationConstraint& type, const Duration& duration);

			void removeDistanceConstraint(const DistanceConstraint& type);
			void removeDurationConstraint(const DurationConstraint& type);

		private:
			std::unique_ptr<Distance> _schedule_distance_limit;
			std::unique_ptr<Duration> _schedule_driving_time_limit;
			std::unique_ptr<Duration> _schedule_working_time_limit;

			std::unique_ptr<Distance> _run_distance_limit;
			std::unique_ptr<Duration> _run_driving_time_limit;
			std::unique_ptr<Duration> _run_working_time_limit;
	};
}