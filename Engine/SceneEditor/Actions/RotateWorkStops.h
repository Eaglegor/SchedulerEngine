#pragma once

#include "../Action.h"

#include "MoveWorkStops.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <SceneEditor_export.h>
#include <cstddef>

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class SCENEEDITOR_EXPORT RotateWorkStops : public Action<void>
	{
	public:
		RotateWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator middle, Run::ConstWorkStopIterator last);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		MoveWorkStops backend;
	};
}
