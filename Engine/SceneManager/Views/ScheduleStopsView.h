#pragma once

#include <cstdint>
#include <vector>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Stop;
	class Schedule;

	class SCHEDULERENGINE_EXPORT ScheduleStopsView
	{
	public:
		ScheduleStopsView(Schedule* schedule);

		const Stop* operator[](size_t index) const;
		Stop* operator[](size_t index);

		std::vector<Stop*>::iterator begin();
		std::vector<Stop*>::iterator end();

		std::vector<Stop*>::const_iterator begin() const;
		std::vector<Stop*>::const_iterator end() const;

		size_t size() const;
		bool empty() const;

	private:
		std::vector<Stop*> stops;
	};
}