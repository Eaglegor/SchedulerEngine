#pragma once

#include "../ScheduleActualizationAlgorithm.h"
#include "../ScheduleActualizationAlgorithmType.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	/// Updates stops allocation times based on routes length, time windows and performer shift
    class SCHEDULERENGINE_EXPORT StopArrivalTimeActualizationAlgorithm : public ScheduleActualizationAlgorithm
    {
    public:
        StopArrivalTimeActualizationAlgorithm(Schedule* schedule);

        virtual void onOperationAdded(const Stop *stop, const Operation *operation) override;

        virtual void onOperationRemoved(const Stop *stop) override;

        virtual void onStopNextRouteChanged(const Stop *stop) override;

        virtual void onStopAdded(const Run *run, const Stop *stop, size_t index) override;

        virtual void onStopRemoved(const Run *run) override;

        virtual void onRunVehicleChanged(const Run *run, const Vehicle *vehicle) override;

        virtual void onRunAdded(const Run *run, size_t index) override;

        virtual void onRunRemoved() override;

        virtual void actualize();

        virtual ScheduleActualizationAlgorithmType getType();

    private:
        bool dirty_flag;

        ScheduleActualizationAlgorithmType type = ScheduleActualizationAlgorithmType::STOP_ARRIVAL_TIME_ACTUALIZER;
    };
}