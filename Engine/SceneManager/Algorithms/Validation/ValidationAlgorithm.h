#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class ViolationsConsumer;
	class Stop;
	class Run;
	class Schedule;

	template <typename T>
	class ValidationAlgorithm : public Algorithm
	{
	public:
		virtual void validate(const T& object, ViolationsConsumer& violations_consumer) const = 0;
	};

	using StopValidationAlgorithm     = ValidationAlgorithm<Stop>;
	using RunValidationAlgorithm      = ValidationAlgorithm<Run>;
	using ScheduleValidationAlgorithm = ValidationAlgorithm<Schedule>;
}