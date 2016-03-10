#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Vehicle;
    class Run;

	/// Is used by the scene to automatically assign vehicles to the created runs
    class SCHEDULERENGINE_EXPORT RunVehicleBinder
    {
    public:
        virtual ~RunVehicleBinder(){}

        virtual bool bindVehicle(Run* run) = 0;
    };
}