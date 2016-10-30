#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/Run.h>
#include <stack>
#include "SwapWorkStops.h"

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

    class SCENEEDITOR_EXPORT ScrambleWorkStops : public Action<void>
	{
	public:
        ScrambleWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		Run& run;
        std::stack<SwapWorkStops> swaps;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator last;
	};
}
