#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class Schedule;
	class WorkStop;

	class SCENEEDITOR_EXPORT SwapRunWorkStops : public Action
	{
	public:
		using RunIterator = ImmutableVector<Run*>::const_iterator;
		using WorkStopIterator = Run::WorkStopsList::const_iterator;

		SwapRunWorkStops(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Schedule* schedule;
		RunIterator run;
		WorkStopIterator first;
		WorkStopIterator second;
	};
}