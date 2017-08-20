#include "DefaultArrivalTimeActualizationAlgorithm.h"
#include "TimeWindowSelectors/FirstTimeWindowSelector/FirstTimeWindowSelector.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Stop.h>
#include <algorithm>
#include <boost/range/iterator_range.hpp>

namespace Scheduler
{
	void DefaultArrivalTimeActualizationAlgorithm::actualize(Schedule& schedule) const
	{
		if(schedule.getRuns( ).empty( )) return;

		Stop& first_stop = schedule.getRuns( ).begin( )->get( ).getStartStop( );

		FirstTimeWindowSelector time_window_selector;

		// First stop start time is a max from schedule's shift start and operation's time window (if any)
		TimeWindow first_time_window = time_window_selector.selectTimeWindow(first_stop);
		first_stop.setStartTime(std::max(schedule.getShift( ).getStartTime( ), first_time_window.getStartTime( )));

		// We need to minimize the total run time, this budget allows us to start run later than from the shift start
		// Budget = how much waiting time inside runs we can compensate by starting work later
		Duration forward_shift_budget = first_time_window.getEndTime( ) - first_stop.getAllocationTime( ).getEndTime( );


		auto current_stop_iter = schedule.getStops( ).begin( );
		auto next_stop_iter    = std::next(current_stop_iter);
		while(current_stop_iter != std::prev(schedule.getStops( ).end( )))
		{
			Stop& current_stop          = *current_stop_iter;
			Stop& next_stop             = *next_stop_iter;
			TimeWindow next_time_window = time_window_selector.selectTimeWindow(next_stop);
			TimePoint next_route_end    = current_stop.getAllocationTime( ).getEndTime( ) + current_stop.getNextRoute( ).getDuration( );

			// If we have detected waiting time because of the next stop time window, we try to start the run later to avoid such waiting
			Duration waiting_time = next_time_window.getStartTime( ) - next_route_end;
			if(waiting_time > Duration(0))
			{
				if(forward_shift_budget > Duration(0))
				{
					Duration shift = std::min(forward_shift_budget, waiting_time);
					for(Stop& s : boost::make_iterator_range(first_stop.getRun( ).findStop(first_stop), next_stop.getRun( ).findStop(next_stop)))
					{
						s.setStartTime(s.getAllocationTime( ).getStartTime( ) + shift);
					}
					forward_shift_budget -= shift;
					next_route_end += shift;
				}
			}
			next_stop.setStartTime(std::max(next_time_window.getStartTime( ), next_route_end));

			// Budget can only get smaller due to it's purpose
			forward_shift_budget = std::min(forward_shift_budget, next_time_window.getEndTime( ) - next_stop.getAllocationTime( ).getEndTime( ));
		}
	}
}
