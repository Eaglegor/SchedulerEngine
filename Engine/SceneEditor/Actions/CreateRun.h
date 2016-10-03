#pragma once

#include "../Action.h"
#include <boost/optional.hpp>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	class CreateRun : public Action
	{
	public:
		CreateRun(Schedule* schedule, Schedule::RunsList::const_iterator pos, const Location& from, const Location& to);
		
		virtual void perform();
		virtual void rollback();
		
		boost::optional<Schedule::RunsList::iterator> result() const;
		
	private:
		Schedule* schedule;
		Schedule::RunsList::const_iterator pos;
		const Location& from;
		const Location& to;
		
		boost::optional<Schedule::RunsList::iterator> resulting_run;
	};
}