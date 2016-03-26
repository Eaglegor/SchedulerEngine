#pragma once

#include "../Action.h"

#include <cstddef>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	class SCENEEDITOR_EXPORT SwapRunWorkStops : public Action
	{
	public:
		SwapRunWorkStops(Run* r, WorkStop* a, WorkStop* b);
		SwapRunWorkStops(Run* r, size_t a_index, size_t b_index);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		static size_t determine_stop_index(WorkStop* stop);
		static size_t determine_run_index(Run* run);

		Schedule* schedule;
		size_t ir;
		size_t ia;
		size_t ib;
	};
}