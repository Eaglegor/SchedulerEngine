#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/Concepts/Duration.h>
#include <memory>

#include "../ConstraintHolder.h"
#include <Engine/Concepts/Site.h>
#include <Engine/SceneManager/Location.h>


namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief Schedule constraints set
	 */
	class ScheduleConstraints
	{
	public:
		/**
		 * @brief Constraint on location
		 * 
		 * @todo Do we really need this wrapper?
		 */
		class LocationConstraint
		{
		public:
			explicit LocationConstraint(const Location& location)
			    : location(location){};
			LocationConstraint(const LocationConstraint& rhs)
			    : location(rhs.location)
			{
			}

			operator const Location&( ) const
			{
				return location;
			}

			const Location& get( ) const
			{
				return location;
			}

			bool operator==(const LocationConstraint& rhs) const
			{
				return location == rhs.location;
			}

			bool operator==(const Location& rhs) const
			{
				return location == rhs;
			}

		private:
			const Location& location;
		};

		/**
		 * @brief Schedule distance limit constraint
		 */
		using ScheduleDistanceLimitConstraint = ConstraintHolder<Distance>;

		/**
		 * @brief Run distance limit constraint
		 */
		using RunDistanceLimitConstraint      = ConstraintHolder<Distance>;

		/**
		 * @brief Schedule driving time limit constraint
		 */
		using ScheduleDrivingTimeLimitConstraint = ConstraintHolder<Duration>;

		/**
		 * @brief Run driving time limit constraint
		 */
		using RunDrivingTimeLimitConstraint      = ConstraintHolder<Duration>;

		/**
		 * @brief Schedule working time limit constraint
		 */
		using ScheduleWorkingTimeLimitConstraint = ConstraintHolder<Duration>;

		/**
		 * @brief Run working time limit constraint
		 */
		using RunWorkingTimeLimitConstraint      = ConstraintHolder<Duration>;

		/**
		 * @brief Schedule shift start location constraint
		 */
		using ScheduleShiftStartLocationConstraint = ConstraintHolder<LocationConstraint>;

		/**
		 * @brief Schedule shift end location constraint
		 */
		using ScheduleShiftEndLocationConstraint   = ConstraintHolder<LocationConstraint>;

		/**
		 * @brief Returns schedule distance limit constraint
		 * 
		 * @return Schedule distance limit constraint
		 */
		const ScheduleDistanceLimitConstraint& scheduleDistanceLimit( ) const;

		/**
		 * @brief Returns schedule distance limit constraint
		 * 
		 * @return Schedule distance limit constraint
		 */
		ScheduleDistanceLimitConstraint& scheduleDistanceLimit( );

		/**
		 * @brief Returns run distance limit constraint
		 * 
		 * @return Run distance limit constraint
		 */
		const RunDistanceLimitConstraint& runDistanceLimit( ) const;

		/**
		 * @brief Returns run distance limit constraint
		 * 
		 * @return Run distance limit constraint
		 */
		RunDistanceLimitConstraint& runDistanceLimit( );

		/**
		 * @brief Returns schedule driving time limit constraint
		 * 
		 * @return Schedule driving time limit constraint
		 */
		const ScheduleDrivingTimeLimitConstraint& scheduleDrivingTimeLimit( ) const;

		/**
		 * @brief Returns schedule driving time limit constraint
		 * 
		 * @return Schedule driving time limit constraint
		 */
		ScheduleDrivingTimeLimitConstraint& scheduleDrivingTimeLimit( );

		/**
		 * @brief Returns run driving time limit constraint
		 * 
		 * @return Run driving time limit constraint
		 */
		const RunDrivingTimeLimitConstraint& runDrivingTimeLimit( ) const;

		/**
		 * @brief Returns run driving time limit constraint
		 * 
		 * @return Run driving time limit constraint
		 */
		RunDrivingTimeLimitConstraint& runDrivingTimeLimit( );

		/**
		 * @brief Returns schedule working time limit constraint
		 * 
		 * @return Schedule working time limit constraint
		 */
		const ScheduleWorkingTimeLimitConstraint& scheduleWorkingTimeLimit( ) const;

		/**
		 * @brief Returns schedule working time limit constraint
		 * 
		 * @return Schedule working time limit constraint
		 */
		ScheduleWorkingTimeLimitConstraint& scheduleWorkingTimeLimit( );

		/**
		 * @brief Returns run working time limit constraint
		 * 
		 * @return Run working time limit constraint
		 */
		const RunWorkingTimeLimitConstraint& runWorkingTimeLimit( ) const;
		RunWorkingTimeLimitConstraint& runWorkingTimeLimit( );

		/**
		 * @brief Returns schedule shift start location constraint
		 * 
		 * @return Schedule shift start location constraint
		 */
		const ScheduleShiftStartLocationConstraint& shiftStartLocation( ) const;

		/**
		 * @brief Returns schedule shift start location constraint
		 * 
		 * @return Schedule shift start location constraint
		 */
		ScheduleShiftStartLocationConstraint& shiftStartLocation( );

		/**
		 * @brief Returns schedule shift end location constraint
		 * 
		 * @return Schedule shift end location constraint
		 */
		const ScheduleShiftEndLocationConstraint& shiftEndLocation( ) const;

		/**
		 * @brief Returns schedule shift end location constraint
		 * 
		 * @return Schedule shift end location constraint
		 */
		ScheduleShiftEndLocationConstraint& shiftEndLocation( );

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
