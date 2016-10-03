#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <boost/optional.hpp>

namespace Scheduler
{
	class AllocateOrder : public Action
	{
	public:
		AllocateOrder(Schedule::RunsList::const_iterator run, Run::WorkStopsList::iterator pos, const Order* order);
		
		virtual void perform();
		virtual void rollback();
		
		boost::optional<Run::WorkStopsList::iterator> getResultWorkStop() const;
		
	private:
		Schedule::RunsList::const_iterator run;
		Run::WorkStopsList::iterator pos;
		const Order* order;
		boost::optional<Run::WorkStopsList::iterator> result_work_stop;
	};
}