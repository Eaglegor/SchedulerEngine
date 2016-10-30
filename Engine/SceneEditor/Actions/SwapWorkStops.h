#pragma once

#include "../Action.h"

#include <cstddef>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class Schedule;
	class WorkStop;

	class SCENEEDITOR_EXPORT SwapWorkStops : public Action<void>
	{
	public:
		SwapWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Run& run;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator second;
	};
}