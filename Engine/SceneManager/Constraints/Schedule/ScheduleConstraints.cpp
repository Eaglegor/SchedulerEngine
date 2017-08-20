#include "ScheduleConstraints.h"


namespace Scheduler
{
	const ScheduleConstraints::ScheduleDistanceLimitConstraint& ScheduleConstraints::scheduleDistanceLimit( ) const
	{
		return schedule_distance_limit_constraint;
	}

	ScheduleConstraints::ScheduleDistanceLimitConstraint& ScheduleConstraints::scheduleDistanceLimit( )
	{
		return schedule_distance_limit_constraint;
	}

	const ScheduleConstraints::RunDistanceLimitConstraint& ScheduleConstraints::runDistanceLimit( ) const
	{
		return run_distance_limit_constraint;
	}

	ScheduleConstraints::RunDistanceLimitConstraint& ScheduleConstraints::runDistanceLimit( )
	{
		return run_distance_limit_constraint;
	}

	const ScheduleConstraints::ScheduleDrivingTimeLimitConstraint& ScheduleConstraints::scheduleDrivingTimeLimit( ) const
	{
		return schedule_driving_time_limit_constraint;
	}

	ScheduleConstraints::ScheduleDrivingTimeLimitConstraint& ScheduleConstraints::scheduleDrivingTimeLimit( )
	{
		return schedule_driving_time_limit_constraint;
	}

	const ScheduleConstraints::RunDrivingTimeLimitConstraint& ScheduleConstraints::runDrivingTimeLimit( ) const
	{
		return run_driving_time_limit_constraint;
	}

	ScheduleConstraints::RunDrivingTimeLimitConstraint& ScheduleConstraints::runDrivingTimeLimit( )
	{
		return run_driving_time_limit_constraint;
	}

	const ScheduleConstraints::ScheduleWorkingTimeLimitConstraint& ScheduleConstraints::scheduleWorkingTimeLimit( ) const
	{
		return schedule_working_time_limit_constraint;
	}

	ScheduleConstraints::ScheduleWorkingTimeLimitConstraint& ScheduleConstraints::scheduleWorkingTimeLimit( )
	{
		return schedule_working_time_limit_constraint;
	}

	const ScheduleConstraints::RunWorkingTimeLimitConstraint& ScheduleConstraints::runWorkingTimeLimit( ) const
	{
		return run_working_time_limit_constraint;
	}

	const ScheduleConstraints::ScheduleShiftStartLocationConstraint& ScheduleConstraints::shiftStartLocation( ) const
	{
		return schedule_shift_start_location_constraint;
	}

	ScheduleConstraints::ScheduleShiftStartLocationConstraint& ScheduleConstraints::shiftStartLocation( )
	{
		return schedule_shift_start_location_constraint;
	}

	const ScheduleConstraints::ScheduleShiftEndLocationConstraint& ScheduleConstraints::shiftEndLocation( ) const
	{
		return schedule_shift_end_location_constraint;
	}

	ScheduleConstraints::ScheduleShiftEndLocationConstraint& ScheduleConstraints::shiftEndLocation( )
	{
		return schedule_shift_end_location_constraint;
	}

	ScheduleConstraints::RunWorkingTimeLimitConstraint& ScheduleConstraints::runWorkingTimeLimit( )
	{
		return run_working_time_limit_constraint;
	}
}
