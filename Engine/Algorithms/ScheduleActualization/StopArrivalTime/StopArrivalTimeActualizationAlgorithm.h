#pragma once

#include <Engine/SceneManager/Extensions/ScheduleActualizationAlgorithm.h>

#include <StopArrivalTimeActualizationAlgorithm_export.h>

namespace Scheduler
{
	/// Updates stops allocation times based on routes length, time windows and performer shift
    class STOPARRIVALTIMEACTUALIZATIONALGORITHM_EXPORT StopArrivalTimeActualizationAlgorithm : public ScheduleActualizationAlgorithm
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

        virtual void actualize() override;

	    static constexpr const char* staticGetName() { return "StopArrivalTime"; }
		virtual const char* getName() const override { return staticGetName(); };

    private:
        bool dirty_flag;
    };
}