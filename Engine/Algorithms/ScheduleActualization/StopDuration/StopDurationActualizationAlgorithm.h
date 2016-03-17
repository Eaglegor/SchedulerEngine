#pragma once

#include <Engine/SceneManager/Extensions/ScheduleActualizationAlgorithm.h>

#include <StopDurationActualizationAlgorithm_export.h>

namespace Scheduler
{
	/// Updates stops durations based on the operations scheduled to be performed at the stop
	class STOPDURATIONACTUALIZATIONALGORITHM_EXPORT StopDurationActualizationAlgorithm : public ScheduleActualizationAlgorithm
	{
	public:
		StopDurationActualizationAlgorithm(Schedule* schedule);

		virtual void onOperationAdded(const Stop *stop, const Operation *operation) override;

		virtual void onOperationRemoved(const Stop *stop) override;

		virtual void onStopNextRouteChanged(const Stop *stop) override;

		virtual void onStopAdded(const Run *run, const Stop *stop, size_t index) override;

		virtual void onStopRemoved(const Run *run) override;

		virtual void onRunVehicleChanged(const Run *run, const Vehicle *vehicle) override;

		virtual void onRunAdded(const Run *run, size_t index) override;

		virtual void onRunRemoved() override;

		virtual void actualize();

	private:
		bool dirty_flag;
	};
}