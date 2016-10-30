#include "CreateRun.h"

namespace Scheduler
{
	CreateRun::CreateRun(Schedule& schedule, Schedule::ConstRunIterator pos, const Location& from, const Location& to):
	schedule(schedule),
	pos(pos),
	from(from),
	to(to)
	{}
	
	Schedule::RunIterator CreateRun::perform()
	{
		resulting_run = schedule.createRun(pos, from, to);
		return resulting_run.value();
	}
	
	void CreateRun::rollback()
	{
		if(resulting_run) schedule.destroyRun(resulting_run.value());
		resulting_run = None;
	}

}