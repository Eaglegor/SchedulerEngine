#include "ScheduleConstraints.h"


namespace Scheduler
{
	const Distance * ScheduleConstraints::scheduleDistanceLimit() const
	{
		return _schedule_distance_limit.get();
	}
	const Duration * ScheduleConstraints::scheduleDrivingTimeLimit() const
	{
		return _schedule_driving_time_limit.get();
	}
	const Duration * ScheduleConstraints::scheduleWorkingTimeLimit() const
	{
		return _schedule_working_time_limit.get();
	}
	const Distance * ScheduleConstraints::runDistanceLimit() const
	{
		return _run_distance_limit.get();
	}
	const Duration * ScheduleConstraints::runDrivingTimeLimit() const
	{
		return _run_driving_time_limit.get();
	}
	const Duration * ScheduleConstraints::runWorkingTimeLimit() const
	{
		return _run_working_time_limit.get();
	}
	void ScheduleConstraints::addConstraint(const DistanceConstraint & type, const Distance & distance)
	{
		switch (type)
		{
		case DistanceConstraint::RUN_DISTANCE_LIMIT:
		{
			_run_distance_limit = std::make_unique<Distance>(distance);
			break;
		}
		case DistanceConstraint::SCHEDULE_DISTANCE_LIMIT:
		{
			_schedule_distance_limit = std::make_unique<Distance>(distance);
			break;
		}
		default:
			break;
		}
	}
	void ScheduleConstraints::addConstraint(const DurationConstraint & type, const Duration & duration)
	{
		switch (type)
		{
		case DurationConstraint::RUN_DRIVING_TIME:
		{
			_run_driving_time_limit = std::make_unique<Duration>(duration);
			break;
		}
		case DurationConstraint::RUN_WORKING_TIME:
		{
			_run_working_time_limit = std::make_unique<Duration>(duration);
			break;
		}
		case DurationConstraint::SCHEDULE_DRIVING_TIME:
		{
			_schedule_driving_time_limit = std::make_unique<Duration>(duration);
			break;
		}
		case DurationConstraint::SCHEDULE_WORKING_TIME:
		{
			_schedule_working_time_limit = std::make_unique<Duration>(duration);
			break;
		}
		default:
			break;
		}
	}

	void ScheduleConstraints::removeDistanceConstraint(const DistanceConstraint & type)
	{
		switch (type)
		{
		case DistanceConstraint::RUN_DISTANCE_LIMIT:
		{
			_run_distance_limit.reset();
			break;
		}
		case DistanceConstraint::SCHEDULE_DISTANCE_LIMIT:
		{
			_schedule_distance_limit.reset();
			break;
		}
		default:
			break;
		}
	}

	void ScheduleConstraints::removeDurationConstraint(const DurationConstraint & type)
	{
		switch (type)
		{
		case DurationConstraint::RUN_DRIVING_TIME:
		{
			_run_driving_time_limit.reset();
			break;
		}
		case DurationConstraint::RUN_WORKING_TIME:
		{
			_run_working_time_limit.reset();
			break;
		}
		case DurationConstraint::SCHEDULE_DRIVING_TIME:
		{
			_schedule_driving_time_limit.reset();
			break;
		}
		case DurationConstraint::SCHEDULE_WORKING_TIME:
		{
			_schedule_working_time_limit.reset();
			break;
		}
		default:
			break;
		}
	}
}