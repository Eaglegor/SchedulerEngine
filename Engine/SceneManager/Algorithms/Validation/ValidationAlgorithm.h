#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class ViolationsConsumer;
	class Stop;
	class Run;
	class Schedule;

	/**
	 * @ingroup constraints
	 * 
	 * @brief Interface of the algorithm used to validate constraints satisfaction
	 * 
	 * @tparam T The object type to be checked
	 */
	template <typename T>
	class ValidationAlgorithm : public Algorithm
	{
	public:
		/**
		 * @brief Checks the object and reports violations to the provided consumer
		 * 
		 * @param object Object to check
		 * @param violations_consumer Violations consumer to report violations to
		 */
		virtual void validate(const T& object, ViolationsConsumer& violations_consumer) const = 0;
	};

	/**
	 * @ingroup constraints
	 * @brief Algorithm used to validate stop
	 */
	using StopValidationAlgorithm     = ValidationAlgorithm<Stop>;

	/**
	 * @ingroup constraints
	 * @brief Algorithm used to validate run
	 */
	using RunValidationAlgorithm      = ValidationAlgorithm<Run>;

	/**
	 * @ingroup constraints
	 * @brief Algorithm used to validate schedule
	 */
	using ScheduleValidationAlgorithm = ValidationAlgorithm<Schedule>;
}