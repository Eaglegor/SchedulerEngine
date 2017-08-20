#include "AllocateOrder.h"

namespace Scheduler
{
	AllocateOrder::AllocateOrder(Run& run, Run::ConstWorkStopIterator pos, const Order& order)
	    : run(run),
	      pos(pos),
	      order(order),
	      performed(false)
	{
	}

	Run::WorkStopIterator AllocateOrder::perform( )
	{
		assert(!performed);
		result_work_stop = run.allocateOrder(pos, order);
		performed        = true;
		return result_work_stop;
	}

	void AllocateOrder::rollback( )
	{
		assert(performed);
		run.unallocateOrder(result_work_stop);
		performed = false;
	}
}
