#pragma once

#include "../Action.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class MoveWorkStops : public Action<void>
	{
	public:
		MoveWorkStops(Run& from_run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last, Run& to_run, Run::ConstWorkStopIterator pos);
		MoveWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last, Run::ConstWorkStopIterator pos);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		Run& from_run;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator last;
		Run& to_run;
		Run::ConstWorkStopIterator pos;
		bool action_needed;
	};
}
