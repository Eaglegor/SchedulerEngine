#include "PerformerAssignedVehicleBinder.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
    PerformerAssignedVehicleBinder::PerformerAssignedVehicleBinder(Scene *scene)
    {
    }

    bool PerformerAssignedVehicleBinder::bindVehicle(Run *run) {
        auto iter = bindings.find(run->getSchedule()->getPerformer());
        if(iter == bindings.end()) return false;
        run->setVehicle(iter->second);
        return true;
    }

    void PerformerAssignedVehicleBinder::assign(const Performer *performer, const Vehicle *vehicle) {
        bindings.emplace(performer, vehicle);
    }

    void PerformerAssignedVehicleBinder::unassign(const Performer *performer, const Vehicle *vehicle) {
        bindings.erase(performer);
    }
}