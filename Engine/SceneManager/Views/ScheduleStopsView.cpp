#include "ScheduleStopsView.h"
#include "RunStopsView.h"
#include "../Schedule.h"
#include <algorithm>

namespace Scheduler
{

	ScheduleStopsView::ScheduleStopsView(Schedule *schedule)
	{
		for(Run* r : schedule->getRuns())
		{
			RunStopsView run_stops_view(r);
			std::copy(run_stops_view.begin(), run_stops_view.end(), std::back_inserter(stops));
		}
	}

	const Stop *ScheduleStopsView::operator[](size_t index) const
	{
		return stops[index];
	}

	Stop *ScheduleStopsView::operator[](size_t index)
	{
		return stops[index];
	}

	std::vector<Stop *>::iterator ScheduleStopsView::begin()
	{
		return stops.begin();
	}

	std::vector<Stop *>::iterator ScheduleStopsView::end()
	{
		return stops.end();
	}

	std::vector<Stop *>::const_iterator ScheduleStopsView::begin() const
	{
		return stops.begin();
	}

	std::vector<Stop *>::const_iterator ScheduleStopsView::end() const
	{
		return stops.end();
	}

	size_t ScheduleStopsView::size() const
	{
		return stops.size();
	}

	bool ScheduleStopsView::empty() const
	{
		return stops.empty();
	}
}