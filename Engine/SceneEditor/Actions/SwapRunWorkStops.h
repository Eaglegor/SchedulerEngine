#pragma once

#include "../Action.h"

#include <SceneEditor_export.h>

namespace Scheduler
{
	class Schedule;
	class Run;
	class Stop;

	class SCENEEDITOR_EXPORT SwapRunWorkStops : public Action
	{
	public:
		SwapRunWorkStops(Run* r, Stop* a, Stop* b);

		virtual void perform() override;
		virtual void rollback() override;

	private:
		static size_t determine_stop_index(Stop* stop);
		static size_t determine_run_index(Run* run);

		Schedule* schedule;
		size_t ir;
		size_t ia;
		size_t ib;
	};
}