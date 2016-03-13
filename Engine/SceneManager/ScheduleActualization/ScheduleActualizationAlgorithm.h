#pragma once

#include <cstdint>
#include "ScheduleActualizationAlgorithmType.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Schedule;
    class Stop;
    class Run;
    class Vehicle;
    class Operation;

	/// The base class for all algorithms actualizing schedule after structural changes
    class SCHEDULERENGINE_EXPORT ScheduleActualizationAlgorithm
    {
    public:
        ScheduleActualizationAlgorithm(Schedule* schedule):schedule(schedule){}
        virtual ~ScheduleActualizationAlgorithm(){}

        virtual void onOperationAdded(const Stop* stop, const Operation* operation){}
        virtual void onOperationRemoved(const Stop* stop){}
        virtual void onStopNextRouteChanged(const Stop* stop){}

        virtual void onStopAdded(const Run* run, const Stop* stop, size_t index){}
        virtual void onStopRemoved(const Run* run){}
        virtual void onRunVehicleChanged(const Run* run, const Vehicle* vehicle){}

        virtual void onRunAdded(const Run* run, size_t index){}
        virtual void onRunRemoved(){}

        virtual void actualize() = 0;

        virtual ScheduleActualizationAlgorithmType getType() = 0;

    protected:
        Schedule* schedule;
    };
}