#include "ScheduleActualizationAlgorithmsFactory.h"

#include "Algorithms/SimpleArrivalTimeActualizationAlgorithm.h"

namespace Scheduler
{

    ScheduleActualizationAlgorithm *ScheduleActualizationAlgorithmsFactory::createAlgorithm(
            const ScheduleActualizationAlgorithmType &type, Schedule* schedule) {

        switch(type)
        {
            case ScheduleActualizationAlgorithmType::SIMPLE_ARRIVAL_TIME:
                return new SimpleArrivalTimeActualizationAlgorithm(schedule);
        }
    }

    void ScheduleActualizationAlgorithmsFactory::destroyAlgorithm(ScheduleActualizationAlgorithm *algorithm) {
        delete algorithm;
    }
}