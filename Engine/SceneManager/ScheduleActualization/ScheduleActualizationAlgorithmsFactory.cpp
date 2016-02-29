#include "ScheduleActualizationAlgorithmsFactory.h"

#include "Algorithms/StopDurationActualizationAlgorithm.h"
#include "Algorithms/StopArrivalTimeActualizationAlgorithm.h"

namespace Scheduler
{

    ScheduleActualizationAlgorithm *ScheduleActualizationAlgorithmsFactory::createAlgorithm(
            const ScheduleActualizationAlgorithmType &type, Schedule* schedule) {

        switch(type)
        {
            case ScheduleActualizationAlgorithmType::STOP_ARRIVAL_TIME_ACTUALIZER:
                return new StopArrivalTimeActualizationAlgorithm(schedule);
			case ScheduleActualizationAlgorithmType::STOP_DURATION_ACTUALIZER:
				return new StopDurationActualizationAlgorithm(schedule);
        }
    }

    void ScheduleActualizationAlgorithmsFactory::destroyAlgorithm(ScheduleActualizationAlgorithm *algorithm) {
        delete algorithm;
    }
}