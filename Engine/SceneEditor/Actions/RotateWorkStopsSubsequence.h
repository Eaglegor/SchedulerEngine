#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class SCENEEDITOR_EXPORT RotateWorkStopsSubsequence : public Action
	{
	public:
		using RunIterator = ImmutableVector<Run*>::const_iterator;
		using WorkStopIterator = ImmutableVector<WorkStop*>::const_iterator;

        RotateWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator middle_stop, WorkStopIterator end_stop);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;

		size_t run_index;
        ptrdiff_t start_index;
        ptrdiff_t middle_index;
        ptrdiff_t end_index;

	};
}
