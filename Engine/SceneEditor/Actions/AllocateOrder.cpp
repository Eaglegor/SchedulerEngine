#include "AllocateOrder.h"

namespace Scheduler
{
	AllocateOrder::AllocateOrder(Schedule::RunsList::const_iterator run, Run::WorkStopsList::iterator pos, const Order* order):
	run(run),
	pos(pos),
	order(order)
	{}
	
	void AllocateOrder::perform()
	{
		Run* r = *run;
		if(order->getStartOperation()) r->allocateStartOperation(order->getStartOperation());
		if(order->getWorkOperation()) result_work_stop = r->createWorkStop(pos, order->getWorkOperation());
		if(order->getEndOperation()) r->allocateEndOperation(order->getEndOperation());
	}
	
	void AllocateOrder::rollback()
	{
		Run* r = *run;
		if(order->getStartOperation()) r->unallocateStartOperation(order->getStartOperation());
		if(order->getWorkOperation()) r->destroyWorkStop(result_work_stop.value());
		if(order->getEndOperation()) r->unallocateEndOperation(order->getEndOperation());
		result_work_stop = boost::none;
	}
	
	boost::optional<Run::WorkStopsList::iterator> AllocateOrder::getResultWorkStop() const
	{
		return result_work_stop;
	}

}