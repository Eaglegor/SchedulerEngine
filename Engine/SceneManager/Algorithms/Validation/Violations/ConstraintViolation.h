#pragma once

namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief Type of the constraint violation
	 * 
	 * @sa @ref supported_constraints
	 */
	enum class ConstraintViolationType
	{
		// Stop violations
		OPERATION_TIME_WINDOWS_VIOLATION,
		PERFORMER_SKILLS_REQUIREMENTS_VIOLATION,
		VEHICLE_ATTRIBUTES_REQUIREMENTS_VIOLATION,

		// Run violations
		RUN_DISTANCE_LIMIT_VIOLATION,
		RUN_DRIVING_TIME_LIMIT_VIOLATION,
		RUN_WORKING_TIME_LIMIT_VIOLATION,
		VEHICLE_AVAILABILITY_WINDOWS_VIOLATION,
		VEHICLE_CAPACITY_VIOLATION,

		// Schedule violations
		PERFORMER_AVAILABILITY_WINDOWS_VIOLATION,
		SCHEDULE_DISTANCE_LIMIT_VIOLATION,
		SCHEDULE_DRIVING_TIME_LIMIT_VIOLATION,
		SCHEDULE_WORKING_TIME_LIMIT_VIOLATION

		// Scene violations
	};

	/**
	 * @ingroup constraints
	 * 
	 * @brief Base class for the constraint violations
	 */
	class ConstraintViolation
	{
	public:
		virtual ~ConstraintViolation( )
		{
		}

		/**
		 * @brief Returns the type of the violation
		 * 
		 * @return Type of the violation
		 */
		virtual ConstraintViolationType getType( ) const = 0;
	};
};