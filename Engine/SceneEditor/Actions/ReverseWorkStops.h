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

	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Reverses the order of stops in the work stops sequence
	 */
	class ReverseWorkStops : public Action<void>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run The run where the sequence of stops is located
		 * @param first Iterator to the first stop in the sequence
		 * @param last Iterator to the stop after the last stop in the sequence
		 */
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