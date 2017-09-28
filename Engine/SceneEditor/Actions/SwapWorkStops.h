#pragma once

#include "../Action.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include <cstddef>

namespace Scheduler
{
	class Schedule;
	class WorkStop;

	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Exchanges the positions of 2 work stops in the single run
	 */
	class SwapWorkStops : public Action<void>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run where the stops are located
		 * @param first Iterator to the first stop
		 * @param second Iterator to the second stop
		 */
		SwapWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second);

		virtual void perform( ) override;
		virtual void rollback( ) override;

	private:
		Run& run;
		Run::ConstWorkStopIterator first;
		Run::ConstWorkStopIterator second;
	};
}