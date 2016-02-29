#pragma once

#include <cstdint>
#include <vector>
#include "ScheduleActualizationAlgorithmType.h"

namespace Scheduler
{
    class Schedule;
    class Run;
    class Stop;
    class Operation;
    class Vehicle;
    class ScheduleActualizationAlgorithm;
    class ScheduleActualizationAlgorithmsFactory;

    class ScheduleActualizer
    {
    public:
        ScheduleActualizer(Schedule* schedule, ScheduleActualizationAlgorithmsFactory* algorithms_factory);
        ~ScheduleActualizer();

        void onOperationAdded(const Stop* stop, const Operation* operation);
        void onOperationRemoved(const Stop* stop);
        void onStopNextRouteChanged(const Stop* stop);

        void onStopAdded(const Run* run, const Stop* stop, size_t index);
        void onStopRemoved(const Run* run);
        void onRunVehicleChanged(const Run* run, const Vehicle* vehicle);

        void onRunAdded(const Run* run, size_t index);
        void onRunRemoved();

        void actualize();

        ScheduleActualizationAlgorithm* createAlgorithm(const ScheduleActualizationAlgorithmType& type);

    private:
        Schedule* schedule;

        ScheduleActualizationAlgorithmsFactory* algorithms_factory;
        std::vector<ScheduleActualizationAlgorithm*> algorithms;

		bool is_actualizing;
    };
}