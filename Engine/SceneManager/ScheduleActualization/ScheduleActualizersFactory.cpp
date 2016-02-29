#include <assert.h>
#include "ScheduleActualizersFactory.h"
#include "ScheduleActualizer.h"

namespace Scheduler
{

    ScheduleActualizersFactory::ScheduleActualizersFactory(ScheduleActualizationAlgorithmsFactory *algorithms_factory):
    algorithms_factory(algorithms_factory){

    }

    ScheduleActualizer *ScheduleActualizersFactory::createScheduleActualizer(Schedule *schedule) {
		ScheduleActualizer* actualizer = new ScheduleActualizer(schedule, algorithms_factory);

		// Creating actualizer according to the actualization model
		actualizer->createAlgorithm(ScheduleActualizationAlgorithmType::STOP_DURATION_ACTUALIZER);
		actualizer->createAlgorithm(ScheduleActualizationAlgorithmType::STOP_ARRIVAL_TIME_ACTUALIZER);

		return actualizer;
    }

    void ScheduleActualizersFactory::destroyScheduleActualizer(ScheduleActualizer *actualizer) {
        delete actualizer;
    }
}