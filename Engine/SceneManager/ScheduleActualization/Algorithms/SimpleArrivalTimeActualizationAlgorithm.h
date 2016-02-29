#pragma once

#include "../ScheduleActualizationAlgorithm.h"
#include "../ScheduleActualizationAlgorithmType.h"

namespace Scheduler
{
    class SimpleArrivalTimeActualizationAlgorithm : public ScheduleActualizationAlgorithm
    {
    public:
        SimpleArrivalTimeActualizationAlgorithm(Schedule* schedule);

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

        ScheduleActualizationAlgorithmType type = ScheduleActualizationAlgorithmType::SIMPLE_ARRIVAL_TIME;
    };
}