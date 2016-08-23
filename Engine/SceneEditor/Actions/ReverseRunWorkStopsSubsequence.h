#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class SCENEEDITOR_EXPORT ReverseWorkStopsSubsequence : public Action
	{
	public:
		using ConstRunIterator = ImmutableVector<Run*>::const_iterator;
		using ConstWorkStopIterator = Run::WorkStopsList::const_iterator;
		using WorkStopIterator = Run::WorkStopsList::iterator;

		ReverseWorkStopsSubsequence(ConstRunIterator run_iterator, ConstWorkStopIterator start_stop, ConstWorkStopIterator end_stop);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;
		ConstRunIterator run;
		WorkStopIterator first;
		WorkStopIterator last;
		WorkStopIterator new_first;
	};
}