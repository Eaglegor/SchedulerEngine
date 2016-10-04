#pragma once

#include "../Action.h"
#include <boost/optional.hpp>
#include <Engine/SceneManager/Schedule.h>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT DestroyRun : public Action
	{
	public:
		DestroyRun(Schedule* schedule, Schedule::RunsList::const_iterator pos);
		virtual ~DestroyRun();
		
		virtual void perform();
		virtual void rollback();
		
		boost::optional<Schedule::RunsList::iterator> result() const;
		
	private:
		Schedule* schedule;
		Schedule::RunsList::const_iterator pos;
		Run* detached_run;
		boost::optional<Schedule::RunsList::iterator> next_iterator;
	};
}