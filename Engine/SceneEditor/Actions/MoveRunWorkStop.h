#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "MoveRunWorkStopsSubsequence.h"
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT MoveRunWorkStop : public Action
	{
	public:
        using ConstRunIterator = ImmutableVector<Run*>::const_iterator;
        using ConstWorkStopIterator = Run::WorkStopsList::iterator;
		using WorkStopIterator = Run::WorkStopsList::const_iterator;
		
        MoveRunWorkStop(ConstRunIterator run_iterator, ConstWorkStopIterator from, ConstWorkStopIterator to);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		MoveRunWorkStopsSubsequence backend;
	};
}
