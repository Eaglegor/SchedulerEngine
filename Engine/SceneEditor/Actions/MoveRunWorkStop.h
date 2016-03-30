#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT MoveRunWorkStop : public Action
	{
	public:
		MoveRunWorkStop(Run* r, WorkStop* from, WorkStop* to);
		MoveRunWorkStop(Run* r, size_t from_index, size_t to_index);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		static size_t determine_stop_index(WorkStop* stop);
		static size_t determine_run_index(Run* run);

		Schedule* schedule;
		size_t irun;
		size_t ifrom;
		size_t ito;
	};
}