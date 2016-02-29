#include <assert.h>
#include "ScheduleActualizersFactory.h"
#include "ScheduleActualizer.h"

namespace Scheduler
{

    ScheduleActualizersFactory::ScheduleActualizersFactory(ScheduleActualizationAlgorithmsFactory *algorithms_factory):
    algorithms_factory(algorithms_factory){

    }

    ScheduleActualizer *ScheduleActualizersFactory::createScheduleActualizer(Schedule *schedule) {
        return new ScheduleActualizer(schedule, algorithms_factory);
    }

    void ScheduleActualizersFactory::destroyScheduleActualizer(ScheduleActualizer *actualizer) {
        delete actualizer;
    }
}