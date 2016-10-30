#include "AllocateOrder.h"

namespace Scheduler
{
	AllocateOrder::AllocateOrder(Run& run, Run::ConstWorkStopIterator pos, const Order& order):
	run(run),
	pos(pos),
	order(order)
	{}
	
	Optional<Run::WorkStopIterator> AllocateOrder::perform()
	{
		if(order.getStartOperation()) run.allocateStartOperation(order.getStartOperation().get());
		if(order.getWorkOperation()) result_work_stop = run.createWorkStop(pos, order.getWorkOperation().get());
		if(order.getEndOperation()) run.allocateEndOperation(order.getEndOperation().get());
		return result_work_stop;
	}
	
	void AllocateOrder::rollback()
	{
		if(order.getStartOperation()) run.unallocateStartOperation(order.getStartOperation().get());
		if(order.getWorkOperation()) run.destroyWorkStop(result_work_stop.get());
		if(order.getEndOperation()) run.unallocateEndOperation(order.getEndOperation().get());
		result_work_stop = None;
	}
}