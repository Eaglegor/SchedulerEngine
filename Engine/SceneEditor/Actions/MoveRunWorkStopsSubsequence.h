#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "RotateWorkStopsSubsequence.h"
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT MoveRunWorkStopsSubsequence : public Action
	{
	public:
        using RunIterator = ImmutableVector<Run*>::const_iterator;
        using WorkStopIterator = Run::WorkStopsList::const_iterator;
        MoveRunWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop, WorkStopIterator new_position);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;
		size_t irun;
		size_t istart_stop;
		size_t iend_stop;
		size_t inew_position;
		RotateWorkStopsSubsequence rotate_command;
	};
}
