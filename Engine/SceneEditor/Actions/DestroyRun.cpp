#include "DestroyRun.h"

namespace Scheduler
{
	DestroyRun::DestroyRun(Schedule& schedule, Schedule::ConstRunIterator pos)
	    : schedule(schedule),
	      pos(pos),
	      detached_run(*pos),
	      performed(false)
	{
	}

	DestroyRun::~DestroyRun( )
	{
		if(performed) destructor(&detached_run);
	}

	Schedule::RunIterator DestroyRun::perform( )
	{
		assert(!performed);

		auto result   = schedule.detachRun(pos);
		next_iterator = result.first;
		destructor    = result.second;
		performed     = true;
		return next_iterator.get( );
	}

	void DestroyRun::rollback( )
	{
		assert(performed);

		schedule.attachRun(next_iterator.value( ), detached_run);
		next_iterator = None;
		performed     = false;
	}
}