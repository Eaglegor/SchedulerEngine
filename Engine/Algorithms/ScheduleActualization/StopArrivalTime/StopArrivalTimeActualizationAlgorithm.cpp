#include "StopArrivalTimeActualizationAlgorithm.h"
#include <Engine/SceneManager/Views/ScheduleStopsView.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Algorithms/TimeWindowSelectors/FirstTimeWindowSelector/FirstTimeWindowSelector.h>

namespace Scheduler
{

    StopArrivalTimeActualizationAlgorithm::StopArrivalTimeActualizationAlgorithm(Schedule* schedule):
    ScheduleActualizationAlgorithm(schedule),
    dirty_flag(true)
    {
    }

    void StopArrivalTimeActualizationAlgorithm::actualize() {
        if(!dirty_flag) return;

		ScheduleStopsView stops(schedule);

		if(stops.empty()) return;

		FirstTimeWindowSelector time_window_selector;

		// First stop start time is a max from schedule's shift start and operation's time window (if any)
		TimeWindow first_time_window = time_window_selector.selectTimeWindow(stops[0]);
		stops[0]->setStartTime(std::max(schedule->getShift().getStartTime(), first_time_window.getStartTime()));

		// We need to minimize the total run time, this budget allows us to start run later than from the shift start
		// Budget = how much waiting time inside runs we can compensate by starting work later
		Duration forward_shift_budget = first_time_window.getEndTime() - stops[0]->getAllocationTime().getEndTime();

		for(size_t i = 0; i < stops.size() - 1; ++i)
		{
			Stop* current_stop = stops[i];
			Stop* next_stop = stops[i + 1];
			TimeWindow next_time_window = time_window_selector.selectTimeWindow(next_stop);
			TimePoint next_route_end = current_stop->getAllocationTime().getEndTime() + current_stop->getNextRoute().getDuration();

			// If we have detected waiting time because of the next stop time window, we try to start the run later to avoid such waiting
			Duration waiting_time = next_time_window.getStartTime() - next_route_end;
			if(waiting_time > Duration(0))
			{
				if(forward_shift_budget > Duration(0))
				{
					Duration shift = std::min(forward_shift_budget, waiting_time);
					for(size_t j = 0; j <= i; ++j)
					{
						stops[j]->setStartTime(stops[j]->getAllocationTime().getStartTime() + shift);
					}
					forward_shift_budget -= shift;
					next_route_end += shift;
				}
			}
			next_stop->setStartTime(std::max(next_time_window.getStartTime(), next_route_end));

			// Budget can only get smaller due to it's purpose
			forward_shift_budget = std::min(forward_shift_budget, next_time_window.getEndTime() - next_stop->getAllocationTime().getEndTime());
		}

		dirty_flag = false;
    }

    void StopArrivalTimeActualizationAlgorithm::onOperationAdded(const Stop *stop, const Operation *operation) {
        dirty_flag = true;
    }

    void StopArrivalTimeActualizationAlgorithm::onOperationRemoved(const Stop *stop) {
        dirty_flag = true;
    }

    void StopArrivalTimeActualizationAlgorithm::onStopAdded(const Run *run, const Stop *stop, size_t index) {
        dirty_flag = true;
    }

	void StopArrivalTimeActualizationAlgorithm::onStopReplaced(const Run * run, const Stop * new_stop, size_t index)
	{
		dirty_flag = true;
	}

    void StopArrivalTimeActualizationAlgorithm::onStopRemoved(const Run *run) {
        dirty_flag = true;
    }

    void StopArrivalTimeActualizationAlgorithm::onRunVehicleChanged(const Run *run, const Vehicle *vehicle) {
        dirty_flag = true;
    }

    void StopArrivalTimeActualizationAlgorithm::onRunAdded(const Run *run, size_t index) {
        dirty_flag = true;
    }

    void StopArrivalTimeActualizationAlgorithm::onRunRemoved() {
        dirty_flag = true;
    }

    void StopArrivalTimeActualizationAlgorithm::onStopNextRouteChanged(const Stop *stop) {
        dirty_flag = true;
    }
}