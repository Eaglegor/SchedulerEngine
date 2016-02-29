#pragma once

#include <Engine/SceneManager/Schedule.h>
#include "ScheduleActualizationAlgorithmType.h"

namespace Scheduler
{
    class ScheduleActualizationAlgorithm;

    class ScheduleActualizationAlgorithmsFactory
    {
    public:
        ScheduleActualizationAlgorithm* createAlgorithm(const ScheduleActualizationAlgorithmType& type, Schedule* schedule);
        void destroyAlgorithm(ScheduleActualizationAlgorithm* algorithm);
    };
}