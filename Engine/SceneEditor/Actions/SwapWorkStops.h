#pragma once

#include "../Action.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>

namespace Scheduler
{
	class Schedule;
	class WorkStop;

	class SwapWorkStops : public Action<void>
	{
	public:
		SwapWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		Run& run;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator second;
	};
}