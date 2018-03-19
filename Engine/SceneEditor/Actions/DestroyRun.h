#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Utils/Optional.h>

#include <boost/optional.hpp>

namespace Scheduler
{
	/**
	 * @ingroup scene_editor_actions
	 * 
	 * @brief Destroys the run
	 */
	class DestroyRun : public Action<Schedule::RunIterator>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param schedule Schedule for which the run has to be destroyed
		 * @param pos Iterator to the run to be destroyed
		 */
		DestroyRun(Schedule& schedule, Schedule::ConstRunIterator pos);
		virtual ~DestroyRun( );

		/**
		 * @brief Destroys the run
		 * 
		 * @details Technically this method detaches the run from the schedule using the detachRun() method
		 * and saves this run to restore it later
		 */
		virtual Schedule::RunIterator perform( ) override;

		/**
		 * @brief Restores the run on the schedule
		 * 
		 * @details Technically this method attaches the previously detached run to the schedule using the attachRun() method
		 */
		virtual void rollback( ) override;

	private:
		Schedule& schedule;
		Schedule::ConstRunIterator pos;
		Schedule::RunDestructor destructor;
		Run& detached_run;
		Optional<Schedule::RunIterator> next_iterator;
		bool performed;
	};
}