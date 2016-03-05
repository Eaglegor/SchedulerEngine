#pragma once

#include <unordered_map>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Performer;
    class Vehicle;

    struct SCHEDULERENGINE_EXPORT PerformerAssignedVehicleBinderSettings
    {
        std::unordered_map<const Performer*, const Vehicle*> assignments;
    };
}
