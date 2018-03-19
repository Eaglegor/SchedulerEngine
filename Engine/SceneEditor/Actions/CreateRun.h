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
	 * @brief Creates new run in the schedule
	 */
	class CreateRun : public Action<Schedule::RunIterator>
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param schedule Schedule where the run has to be created
		 * @param pos Position in the schedule where the run will be created
		 * @param from Start location of the run
		 * @param to End location of the run
		 */
		CreateRun(Schedule& schedule, Schedule::ConstRunIterator pos, const Location& from, const Location& to);

		virtual Schedule::RunIterator perform( ) override;
		virtual void rollback( ) override;

	private:
		Schedule& schedule;
		Schedule::ConstRunIterator pos;
		const Location& from;
		const Location& to;
		Optional<Schedule::RunIterator> resulting_run;
	};
}