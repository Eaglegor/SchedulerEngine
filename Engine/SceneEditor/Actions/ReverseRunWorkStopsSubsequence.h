#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class SCENEEDITOR_EXPORT ReverseWorkStopsSubsequence : public Action
	{
	public:
		ReverseWorkStopsSubsequence(Run *r, WorkStop* start_stop, WorkStop* end_stop);
		ReverseWorkStopsSubsequence(Run *r, size_t start_index, size_t end_index);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		static size_t determine_stop_index(WorkStop* stop);
		static size_t determine_run_index(Run* run);

		Schedule* schedule;

		size_t run_index;
		size_t start_index;
		size_t end_index;

	};
}