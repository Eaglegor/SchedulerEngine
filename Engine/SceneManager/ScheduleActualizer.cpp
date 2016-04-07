#include <assert.h>
#include "ScheduleActualizer.h"
#include "Extensions/ScheduleActualizationAlgorithm.h"

namespace Scheduler
{

    ScheduleActualizer::ScheduleActualizer(Schedule *schedule):
    schedule(schedule),
    algorithms_factory(nullptr),
	is_actualizing(false)
    {
    }

	ScheduleActualizer::ScheduleActualizer(Schedule * schedule, const ScheduleActualizer & rhs):
		schedule(schedule),
		algorithms_factory(rhs.algorithms_factory),
		is_actualizing(false)
	{
		for (ScheduleActualizationAlgorithm* algorithm : rhs.algorithms)
		{
			algorithms.push_back(algorithm->clone(schedule, algorithms_factory));
		}
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

    void ScheduleActualizer::onStopRemoved(const Run *run, size_t index) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onStopRemoved(run, index);
    }

	void ScheduleActualizer::onStopReplaced(const Run * run, const Stop * new_stop, size_t index)
	{
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onStopReplaced(run, new_stop, index);
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

    ScheduleActualizer::~ScheduleActualizer() {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithms_factory->destroyObject(algorithm);
    }

    void ScheduleActualizer::onStopNextRouteChanged(const Stop *stop) {
        for(ScheduleActualizationAlgorithm* algorithm : algorithms) algorithm->onStopNextRouteChanged(stop);
    }

    void ScheduleActualizer::setScheduleActualizationAlgorithmsFactory(
            Factory<ScheduleActualizationAlgorithm> *factory) {
        this->algorithms_factory = factory;
    }
}