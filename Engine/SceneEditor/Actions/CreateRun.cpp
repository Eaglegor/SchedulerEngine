#include "CreateRun.h"

namespace Scheduler
{
	CreateRun::CreateRun(Schedule* schedule, Schedule::RunsList::const_iterator pos, const Location& from, const Location& to):
	schedule(schedule),
	pos(pos),
	from(from),
	to(to)
	{}
	
	void CreateRun::perform()
	{
		resulting_run = schedule->createRun(pos, from, to);
	}
	
	void CreateRun::rollback()
	{
		schedule->destroyRun(resulting_run.value());
		resulting_run = boost::none;
	}
	
	boost::optional<Schedule::RunsList::iterator> CreateRun::result() const
	{
		return resulting_run;
	}


}