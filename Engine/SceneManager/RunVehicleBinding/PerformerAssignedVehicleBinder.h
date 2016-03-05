#pragma once

#include "RunVehicleBinder.h"
#include <unordered_map>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Performer;
    class Vehicle;
    class Scene;
    class PerformerAssignedVehicleBinderSettings;

    class SCHEDULERENGINE_EXPORT PerformerAssignedVehicleBinder : public RunVehicleBinder
    {
    public:
        PerformerAssignedVehicleBinder(Scene* scene, const PerformerAssignedVehicleBinderSettings &settings);

        virtual bool bindVehicle(Run *run) override;

        void assign(const Performer *performer, const Vehicle *vehicle);
        void unassign(const Performer *performer, const Vehicle *vehicle);

    private:
        std::unordered_map<const Performer*, const Vehicle*> bindings;
    };
}
