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

	class SCENEEDITOR_EXPORT ReverseWorkStopsSubsequence : public Action
	{
	public:
		using RunIterator = ImmutableVector<Run*>::const_iterator;
		using WorkStopIterator = ImmutableVector<WorkStop*>::const_iterator;

		ReverseWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;

		size_t run_index;
		size_t start_index;
		size_t end_index;

	};
}