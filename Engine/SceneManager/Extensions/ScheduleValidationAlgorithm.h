#pragma once

namespace Scheduler
{
	class Schedule;
	
	class ScheduleValidationAlgorithm
	{
		public:
			virtual bool isValid(const Schedule* schedule) const = 0;
	};
}