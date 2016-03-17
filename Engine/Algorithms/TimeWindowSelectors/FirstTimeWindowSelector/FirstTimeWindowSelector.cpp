#include "FirstTimeWindowSelector.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Operation.h>

namespace Scheduler
{
	TimeWindow FirstTimeWindowSelector::selectTimeWindow(Stop *stop)
	{
		if(stop->getOperations().empty()) return TimeWindow();

		const ImmutableVector<TimeWindow>& time_windows = (*(stop->getOperations().begin()))->getTimeWindows();
		if(time_windows.empty()) return TimeWindow();
		return time_windows[0];
	}
}