#pragma once

#include "../Action.h"

#include "SwapWorkStops.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>
#include <stack>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Shuffles work stops in the specified stops sequence
	 */
	class ScrambleWorkStops : public Action<void>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run where the stops sequence is located
		 * @param first Iterator to the first stop in the sequence
		 * @param last Iterator to the stop after the last stop in the sequence
		 */
		ScrambleWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		Run& run;
		std::vector<SwapWorkStops> swaps;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator last;
		bool performed;
	};
}
