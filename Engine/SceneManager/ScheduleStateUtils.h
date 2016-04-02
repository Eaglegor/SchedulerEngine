#pragma once

namespace Scheduler
{
	class Schedule;

	class ScheduleStateUtils
	{
	public:
		static void copyState(const Schedule* from, Schedule* to);
	};
}