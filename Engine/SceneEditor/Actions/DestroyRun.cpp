#include "DestroyRun.h"

namespace Scheduler
{
	DestroyRun::DestroyRun(Schedule* schedule, Schedule::RunsList::const_iterator pos):
	schedule(schedule),
	pos(pos),
	detached_run(nullptr)
	{}
	
	DestroyRun::~DestroyRun()
	{
		if(detached_run) schedule->destroyDetachedRun(detached_run);
	}

	
	void DestroyRun::perform()
	{
		detached_run = *pos;
		next_iterator = schedule->detachRun(pos);
	}

	void DestroyRun::rollback()
	{
		if(detached_run) schedule->attachRun(next_iterator.value(), detached_run);
		next_iterator = boost::none;
		detached_run = nullptr;
	}
	
	boost::optional< Schedule::RunsList::iterator > DestroyRun::result() const
	{
		return next_iterator;
	}


}