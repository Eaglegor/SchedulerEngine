#pragma once

#include <unordered_map>

namespace Scheduler
{
    class Performer;
    class Vehicle;

    struct PerformerAssignedVehicleBinderSettings
    {
        std::unordered_map<const Performer*, const Vehicle*> assignments;
    };
}
