#include <assert.h>
#include "ScheduleActualizer.h"
#include "ScheduleActualizationAlgorithmsFactory.h"
#include "ScheduleActualizationAlgorithm.h"

namespace Scheduler
{

    ScheduleActualizer::ScheduleActualizer(Schedule *schedule, ScheduleActualizationAlgorithmsFactory* algorithms_factory):
    schedule(schedule),
    algorithms_factory(algorithms_factory),
	is_actualizing(false)
    {
    }

    void ScheduleActualizer::onOperationAdded(const Stop *stop, const Operation *operation) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onOperationAdded(stop, operation);
    }

    void ScheduleActualizer::onOperationRemoved(const Stop *stop) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onOperationRemoved(stop);
    }

    void ScheduleActualizer::onStopAdded(const Run *run, const Stop *stop, size_t index) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onStopAdded(run, stop, index);
    }

    void ScheduleActualizer::onStopRemoved(const Run *run) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onStopRemoved(run);
    }

    void ScheduleActualizer::onRunVehicleChanged(const Run *run, const Vehicle *vehicle) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onRunVehicleChanged(run, vehicle);
    }

    void ScheduleActualizer::onRunAdded(const Run *run, size_t index) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onRunAdded(run, index);
    }

    void ScheduleActualizer::onRunRemoved() {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onRunRemoved();
    }

    void ScheduleActualizer::actualize() {
		if(is_actualizing) return;
		is_actualizing = true;
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->actualize();
		is_actualizing = false;
    }

    ScheduleActualizationAlgorithm* ScheduleActualizer::createAlgorithm(const ScheduleActualizationAlgorithmType &type) {
        assert(algorithms_factory);

        ScheduleActualizationAlgorithm* algorithm = algorithms_factory->createAlgorithm(type, schedule);

        if(algorithm == nullptr) return nullptr;

        algorithms.push_back(algorithm);
        return algorithm;
    }

    ScheduleActualizer::~ScheduleActualizer() {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithms_factory->destroyAlgorithm(algorithm);
    }

    void ScheduleActualizer::onStopNextRouteChanged(const Stop *stop) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onStopNextRouteChanged(stop);
    }
}