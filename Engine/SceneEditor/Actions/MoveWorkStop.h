#pragma once

#include "../Action.h"

#include "MoveWorkStops.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>

namespace Scheduler
{
	class Schedule;
	class Run;
	class WorkStop;

	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Transfers the single work stop from one run into the another run 
	 * or into the another position in the same run
	 */
	class MoveWorkStop : public Action<void>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param from_run The run to transfer the stop from
		 * @param from_pos Iterator to the stop to transfer
		 * @param to_run The run to transfer the stop into
		 * @param to_pos The new position of the stop
		 */
		MoveWorkStop(Run& from_run, Run::ConstWorkStopIterator from_pos, Run& to_run, Run::ConstWorkStopIterator to_pos);

		/**
		 * @brief Constructor
		 * 
		 * @param run The run to transfer the stop from
		 * @param from Iterator to the stop to transfer
		 * @param to The new position of the stop
		 */
		MoveWorkStop(Run& run, Run::ConstWorkStopIterator from, Run::ConstWorkStopIterator to);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		bool action_needed;
		MoveWorkStops backend;
	};
}
