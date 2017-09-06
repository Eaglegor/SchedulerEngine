#pragma once

#include "../Action.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	class ReverseWorkStops : public Action<void>
	{
	public:
		ReverseWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		Run& run;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator last;
		Run::ConstWorkStopIterator new_first;
	};
}