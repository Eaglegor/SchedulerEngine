#include "SimpleArrivalTimeActualizationAlgorithm.h"

namespace Scheduler
{

    SimpleArrivalTimeActualizationAlgorithm::SimpleArrivalTimeActualizationAlgorithm(Schedule* schedule):
    ScheduleActualizationAlgorithm(schedule),
    dirty_flag(true)
    {
    }

    void SimpleArrivalTimeActualizationAlgorithm::actualize() {
        if(!dirty_flag) return;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onOperationAdded(const Stop *stop, const Operation *operation) {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onOperationRemoved(const Stop *stop) {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onStopAdded(const Run *run, const Stop *stop, size_t index) {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onStopRemoved(const Run *run) {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onRunVehicleChanged(const Run *run, const Vehicle *vehicle) {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onRunAdded(const Run *run, size_t index) {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onRunRemoved() {
        dirty_flag = true;
    }

    void SimpleArrivalTimeActualizationAlgorithm::onStopNextRouteChanged(const Stop *stop) {
        dirty_flag = true;
    }

    ScheduleActualizationAlgorithmType SimpleArrivalTimeActualizationAlgorithm::getType() {
        return type;
    }
}