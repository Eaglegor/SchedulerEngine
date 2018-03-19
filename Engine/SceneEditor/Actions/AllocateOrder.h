#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Utils/Optional.h>


namespace Scheduler
{
	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Allocates order into the run
	 */
	class AllocateOrder : public Action<Run::WorkStopIterator>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run to allocate order into
		 * @param pos Position in the run where the work stop will be created
		 * @param order The order to allocate into the run
		 */
		AllocateOrder(Run& run, Run::ConstWorkStopIterator pos, const Order& order);

		virtual Run::WorkStopIterator perform( ) override;
		virtual void rollback( ) override;

	private:
		bool performed;
		Run& run;
		Run::ConstWorkStopIterator pos;
		const Order& order;
		Run::WorkStopIterator result_work_stop;
	};
}
