#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Schedule;
	
	class ScheduleValidationAlgorithm : public Algorithm
	{
		public:
			virtual bool isValid(const Schedule& schedule) const = 0;
	};
}