#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "MoveRunWorkStopsSubsequence.h"

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT MoveRunWorkStop : public Action
	{
	public:
        using RunIterator = ImmutableVector<Run*>::const_iterator;
        using WorkStopIterator = ImmutableVector<WorkStop*>::const_iterator;
        MoveRunWorkStop(RunIterator run_iterator, WorkStopIterator from, WorkStopIterator to);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;
		size_t irun;
		size_t ifrom;
		size_t ito;
		MoveRunWorkStopsSubsequence move_command;
	};
}
