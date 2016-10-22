#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "MoveWorkStops.h"
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT MoveWorkStop : public Action<void>
	{
	public:
		MoveWorkStop(Run& from_run, Run::ConstWorkStopIterator from_pos, Run& to_run, Run::ConstWorkStopIterator to_pos);
        MoveWorkStop(Run& run, Run::ConstWorkStopIterator from, Run::ConstWorkStopIterator to);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		bool action_needed;
		MoveWorkStops backend;
	};
}
