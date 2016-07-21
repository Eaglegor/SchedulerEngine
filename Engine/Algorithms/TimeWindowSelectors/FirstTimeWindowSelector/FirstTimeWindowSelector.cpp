#include "FirstTimeWindowSelector.h"
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/RunBoundaryStop.h>
#include <Engine/SceneManager/StopVisitor.h>
#include <Engine/SceneManager/Operation.h>

namespace Scheduler
{
	class SImpl : public StopVisitor
	{
	public:
		SImpl(TimeWindow& out_time_window):out_time_window(out_time_window) {}

		virtual void dispatch(WorkStop * work_stop) override
		{
			assert(work_stop->getOperation());

			const ImmutableVector<TimeWindow>& time_windows = work_stop->getOperation()->constraints().timeWindows();
			if (time_windows.empty()) return;
			out_time_window = time_windows[0];
		}

		virtual void dispatch(RunBoundaryStop * run_boundary_stop) override
		{
			if (run_boundary_stop->getOperations().empty()) return;

			const ImmutableVector<TimeWindow>& time_windows = (*(run_boundary_stop->getOperations().begin()))->constraints().timeWindows();
			if (time_windows.empty()) return;
			out_time_window = time_windows[0];
		}

	private:
		TimeWindow &out_time_window;
	};

	TimeWindow FirstTimeWindowSelector::selectTimeWindow(Stop *stop)
	{
		TimeWindow tw;
		SImpl selector(tw);
		stop->acceptVisitor(&selector);
		return tw;
	}
}
