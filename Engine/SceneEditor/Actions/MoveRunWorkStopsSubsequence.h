#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT MoveRunWorkStopsSubsequence : public Action
	{
	public:
        using ConstRunIterator = ImmutableVector<Run*>::const_iterator;
        using ConstWorkStopIterator = Run::WorkStopsList::const_iterator;
		using WorkStopIterator = Run::WorkStopsList::iterator;
		
        MoveRunWorkStopsSubsequence(ConstRunIterator run_iterator, ConstWorkStopIterator start_stop, ConstWorkStopIterator end_stop, ConstWorkStopIterator new_position);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;
		ConstRunIterator run;
		WorkStopIterator new_position;
		WorkStopIterator first;
		WorkStopIterator last;
		bool action_needed;
	};
}
