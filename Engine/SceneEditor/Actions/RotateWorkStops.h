#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>
#include "MoveWorkStops.h"

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class SCENEEDITOR_EXPORT RotateWorkStops : public Action<void>
	{
	public:
        RotateWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator middle, Run::ConstWorkStopIterator last);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		MoveWorkStops backend;
	};
}
