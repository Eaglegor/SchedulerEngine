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

    class SCENEEDITOR_EXPORT ScrambleWorkStopsSubsequence : public Action
	{
	public:
		using RunIterator = ImmutableVector<Run*>::const_iterator;
		using WorkStopIterator = Run::WorkStopsList::const_iterator;

        ScrambleWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop);

		virtual void perform() override;
		virtual void rollback() override;

	private:

		Schedule* schedule;
        std::vector<size_t> rollback_indexes;
		size_t ir;
		size_t ia;
		size_t ib;
	};
}
