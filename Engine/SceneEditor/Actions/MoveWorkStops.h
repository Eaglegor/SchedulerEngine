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

	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Transfers the sequence of work stops from one run into the another run 
	 * or into the another position in the same run
	 */
	class MoveWorkStops : public Action<void>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param from_run The run to transfer the stops from
		 * @param first Iterator to the first stop to transfer
		 * @param last Iterator to the stop after the last stop to transfer
		 * @param to_run The run to transfer the stops into
		 * @param pos The new position of the stops
		 */
		MoveWorkStops(Run& from_run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last, Run& to_run, Run::ConstWorkStopIterator pos);

		/**
		 * @brief Constructor
		 * 
		 * @param run The run to transfer the stops from
		 * @param first Iterator to the first stop to transfer
		 * @param last Iterator to the stop after the last stop to transfer
		 * @param pos The new position of the stops
		 */
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
