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

		class SCENEMANAGER_EXPORT iterator
		{
		public:
			iterator(ScheduleStopsView& view);
			iterator(ScheduleStopsView& view, size_t current_index);
			iterator(const iterator& rhs);

			iterator& operator=(const iterator& rhs);
			iterator& operator++();
			iterator& operator--();
			iterator operator++(int);
			iterator operator--(int);

			bool operator==(const iterator& iter) const;
			bool operator!=(const iterator& iter) const;

			Stop* operator*();

			size_t index() const;

		private:
			ScheduleStopsView& schedule_view;
			size_t current_index;
		};

		const iterator& begin();
		const iterator& end();

		size_t size() const;
		bool empty() const;
		

	private:
		Schedule* schedule;

		iterator begin_iterator;
		mutable iterator end_iterator;
	};
}