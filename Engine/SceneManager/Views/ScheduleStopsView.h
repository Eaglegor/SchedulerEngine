#pragma once

#include <cstddef>
#include <vector>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Stop;
	class Schedule;

	/**
		This is a linear vector representation of schedule's stops.
		All stops of all runs are represented by this view as a single linear structure.
	*/
	class SCENEMANAGER_EXPORT ScheduleStopsView
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