#include "FirstTimeWindowSelector.h"
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/RunBoundaryStop.h>
#include <Engine/SceneManager/ConstStopVisitor.h>
#include <Engine/SceneManager/Operation.h>

namespace Scheduler
{
	class SImpl : public ConstStopVisitor
	{
	public:
		explicit SImpl(TimeWindow& out_time_window):out_time_window(out_time_window) {}

		virtual void dispatch(const WorkStop& work_stop) override
		{
			const auto& time_windows = work_stop.getOperation().constraints().timeWindows();
			if (time_windows.get().empty()) return;
			out_time_window = time_windows.get()[0];
		}

		virtual void dispatch(const RunBoundaryStop& run_boundary_stop) override
		{
			if (run_boundary_stop.getOperations().empty()) return;

			const auto& time_windows = run_boundary_stop.getOperations().begin()->get().constraints().timeWindows();
			if (time_windows.get().empty()) return;
			out_time_window = time_windows.get()[0];
		}

	private:
		TimeWindow &out_time_window;
	};

	TimeWindow FirstTimeWindowSelector::selectTimeWindow(const Stop &stop) const
	{
		TimeWindow tw;
		SImpl selector(tw);
		stop.acceptVisitor(selector);
		return tw;
	}
}
