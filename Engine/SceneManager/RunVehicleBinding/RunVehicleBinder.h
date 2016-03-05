#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Vehicle;
    class Run;

    class SCHEDULERENGINE_EXPORT RunVehicleBinder
    {
    public:
        virtual ~RunVehicleBinder(){}

        virtual bool bindVehicle(Run* run) = 0;
    };
}