#include <assert.h>
#include "ListBasedRun.h"
#include "WorkStop.h"
#include <iostream>

namespace Scheduler {

    ListBasedRun::ListBasedRun(size_t id, const Location &start_location, const Location &end_location, Schedule *schedule) :
			Run(id, start_location, end_location, schedule)
    {
    }

/*
    const ImmutableVector<WorkStop*>& ListBasedRun::getWorkStops() const {
        return work_stops;
    }

    ImmutableVector<WorkStop *>& ListBasedRun::getWorkStops() {
        return work_stops;
    }
	*/

    /*WorkStop *ListBasedRun::allocateWorkOperation(const Operation *operation) {
        return allocateWorkOperation(operation, work_stops.size());
    }

    WorkStop *ListBasedRun::allocateWorkOperation(const Operation *operation, size_t index) {
        assert(stops_factory);
        assert(index >= 0 && index <= work_stops.size());
        if (!stops_factory) return nullptr;

		WorkStop *stop = createWorkStop(operation);

		Stop* prev_stop = nullptr;
		Stop* next_stop = nullptr;

		if(index == 0) prev_stop = &start_stop;
		else prev_stop = work_stops[index - 1];
		if(index == work_stops.size()) next_stop = &end_stop;
		else next_stop = work_stops[index];

		prev_stop->setNextStop(stop);
		stop->setPrevStop(prev_stop);
		stop->setNextStop(next_stop);
		next_stop->setPrevStop(stop);

        work_stops.insert(work_stops.begin() + index, stop);

        schedule_actualizer->onStopAdded(this, stop, index);

        invalidateWorkStopRoutes(index);

        return stop;
    }*/

    /*void ListBasedRun::unallocateWorkOperation(const Operation *operation, size_t hint) {
        for (size_t i = hint; i < work_stops.size(); ++i) {
            if (work_stops[i]->getOperation() == operation) {
				unallocateWorkOperationAt(i);
                return;
            }
        }
        for (size_t i = 0; i < hint; ++i) {
            if (work_stops[i]->getOperation() == operation) {
				unallocateWorkOperationAt(i);
				return;
            }
        }
    }

    void ListBasedRun::unallocateWorkOperationAt(size_t index) {
        assert(stops_factory);
        assert(index >=0 && index < work_stops.size());

		Stop* old_stop = work_stops[index];
		Stop* prev_stop = old_stop->getPrevStop();
		Stop* next_stop = old_stop->getNextStop();

		prev_stop->setNextStop(next_stop);
		next_stop->setPrevStop(prev_stop);

        stops_factory->destroyObject(work_stops[index]);
        work_stops.erase(work_stops.begin() + index);

        if(work_stops.empty()) start_stop.invalidateRoute();
        else invalidateWorkStopRoutes(index > 0 ? index - 1 : 0);

        schedule_actualizer->onStopRemoved(this, index);
    }*/

	Stop * ListBasedRun::replaceWorkOperation(ImmutableList<WorkStop*>::iterator iter, const Operation* new_operation)
	{
		assert(stops_factory);
		
		if (!stops_factory) return nullptr;

		WorkStop* stop = *iter;

		stop->setOperation(new_operation);

		schedule_actualizer->onStopReplaced(this, stop, std::distance(iter, work_stops.begin());

		invalidateWorkStopRoutes(index);

		return stop;
	}

	void ListBasedRun::invalidateRoutes()
	{
		getStartStop()->invalidateRoute();
		for (WorkStop* stop : work_stops)
		{
			stop->invalidateRoute();
		}
	}

	void ListBasedRun::invalidateWorkStopRoutes(ImmutableList<WorkStop*>::iterator iter) 
	{
		assert(index >= 0 && index < work_stops.size());

		if (iter == work_stops.begin())
		{
			getStartStop()->invalidateRoute();
		}
		else
		{
			ImmutableList<WorkStop*>::iterator prev = iter;
			--prev;
			(*prev)->invalidateRoute();
		}

		(*iter)->invalidateRoute();
	}

	ListBasedRun::~ListBasedRun() {
		assert(stops_factory);

		for(WorkStop* stop : work_stops)
		{
			stops_factory->destroyObject(stop);
		}
	}
}