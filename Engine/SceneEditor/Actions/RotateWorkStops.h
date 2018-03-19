#pragma once

#include "../Action.h"

#include "MoveWorkStops.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>

namespace Scheduler
{
	class WorkStop;
	class Run;
	class Schedule;

	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Performs the circular shift for the work stops sequence
	 */
	class RotateWorkStops : public Action<void>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run where the stops sequence is located
		 * @param first Iterator to the first stop in the sequence
		 * @param middle Iterator to the new first stop in the sequence after the circular shift
		 * @param last Iterator to the stop after the last stop in the sequence
		 */
		RotateWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator middle, Run::ConstWorkStopIterator last);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		MoveWorkStops backend;
	};
}
