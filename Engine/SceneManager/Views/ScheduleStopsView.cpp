#include "ScheduleStopsView.h"
#include "RunStopsView.h"
#include "../Schedule.h"
#include <algorithm>
#include <iterator>

namespace Scheduler
{

	ScheduleStopsView::ScheduleStopsView(Schedule *schedule):
	schedule(schedule),
	begin_iterator(*this),
	end_iterator(*this, size())
	{
	}

	const Stop *ScheduleStopsView::operator[](size_t index) const
	{
		size_t index_left = index;
		for (Run* run : schedule->getRuns())
		{
			RunStopsView view(run);
			if (view.size() >= index_left)
			{
				return view[index_left];
			}
			else index_left -= view.size();
		}
		return nullptr;
	}

	Stop *ScheduleStopsView::operator[](size_t index)
	{
		size_t index_left = index;
		for (Run* run : schedule->getRuns())
		{
			RunStopsView view(run);
			if (view.size() >= index_left)
			{
				return view[index_left];
			}
			else index_left -= view.size();
		}
		return nullptr;
	}

	const ScheduleStopsView::iterator & ScheduleStopsView::begin()
	{
		return begin_iterator;
	}

	const ScheduleStopsView::iterator & ScheduleStopsView::end()
	{
		return end_iterator;
	}

	size_t ScheduleStopsView::size() const
	{
		size_t sum_size = 0;
		for (Run* run : schedule->getRuns())
		{
			sum_size += RunStopsView(run).size();
		}
		return sum_size;
	}

	bool ScheduleStopsView::empty() const
	{
		return schedule->getRuns().empty();
	}


	ScheduleStopsView::iterator::iterator(ScheduleStopsView& view) :
		schedule_view(view),
		current_index(0)
	{
	
	}

	ScheduleStopsView::iterator::iterator(ScheduleStopsView& view, size_t current_index):
		schedule_view(view),
		current_index(current_index)
	{

	}

	ScheduleStopsView::iterator::iterator(const ScheduleStopsView::iterator& rhs) :
		schedule_view(rhs.schedule_view),
		current_index(rhs.current_index)
	{
	
	}

	ScheduleStopsView::iterator& ScheduleStopsView::iterator::operator = (const ScheduleStopsView::iterator& rhs)
	{
		this->schedule_view = rhs.schedule_view;
		this->current_index = rhs.current_index;
		return *this;
	}

	ScheduleStopsView::iterator& ScheduleStopsView::iterator::operator++()
	{
		++this->current_index;
		return *this;
	}

	ScheduleStopsView::iterator& ScheduleStopsView::iterator::operator--() 
	{
		--this->current_index;
		return *this;
	}

	ScheduleStopsView::iterator ScheduleStopsView::iterator::operator++(int)
	{
		iterator iter(*this);
		++this->current_index;
		return iter;
	}

	ScheduleStopsView::iterator ScheduleStopsView::iterator::operator--(int) 
	{
		iterator iter(*this);
		--this->current_index;
		return iter;
	}

	bool ScheduleStopsView::iterator::operator==(const iterator & iter) const
	{
		return &iter.schedule_view == &this->schedule_view && iter.current_index == this->current_index;
	}

	bool ScheduleStopsView::iterator::operator!=(const iterator & iter) const
	{
		return !(*this == iter);
	}
	
	Stop* ScheduleStopsView::iterator::operator*() 
	{
		return schedule_view[current_index];
	}

	size_t ScheduleStopsView::iterator::index() const 
	{
		return current_index;
	}

}