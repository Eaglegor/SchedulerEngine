#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Utils/Optional.h>

#include <boost/optional.hpp>

namespace Scheduler
{
	class DestroyRun : public Action<Schedule::RunIterator>
	{
	public:
		DestroyRun(Schedule& schedule, Schedule::ConstRunIterator pos);
		virtual ~DestroyRun( );

		virtual Schedule::RunIterator perform( ) override;
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