#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>
#include "MoveRunWorkStopsSubsequence.h"

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class SCENEEDITOR_EXPORT RotateWorkStopsSubsequence : public Action
	{
	public:
		using ConstRunIterator = ImmutableVector<Run*>::const_iterator;
		using ConstWorkStopIterator = Run::WorkStopsList::const_iterator;
		using WorkStopIterator = Run::WorkStopsList::iterator;

        RotateWorkStopsSubsequence(ConstRunIterator run_iterator, ConstWorkStopIterator start_stop, ConstWorkStopIterator middle_stop, ConstWorkStopIterator end_stop);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		MoveRunWorkStopsSubsequence backend;
	};
}
