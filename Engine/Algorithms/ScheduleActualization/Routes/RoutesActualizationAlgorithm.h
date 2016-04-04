#pragma once

#include <Engine/SceneManager/Extensions/ScheduleActualizationAlgorithm.h>

#include <RoutesActualizationAlgorithm_export.h>

namespace Scheduler
{
	class RoutingService;

	/// Updates stops durations based on the operations scheduled to be performed at the stop
	class ROUTESACTUALIZATIONALGORITHM_EXPORT RoutesActualizationAlgorithm : public ScheduleActualizationAlgorithm
	{
	public:
		RoutesActualizationAlgorithm(Schedule* schedule, RoutingService* routing_service);

		virtual void onOperationAdded(const Stop *stop, const Operation *operation) override;

		virtual void onOperationRemoved(const Stop *stop) override;

		virtual void onStopNextRouteChanged(const Stop *stop) override;

		virtual void onStopAdded(const Run *run, const Stop *stop, size_t index) override;

		virtual void onStopReplaced(const Run* run, const Stop* new_stop, size_t index) override;

		virtual void onStopRemoved(const Run *run, size_t index) override;

		virtual void onRunVehicleChanged(const Run *run, const Vehicle *vehicle) override;

		virtual void onRunAdded(const Run *run, size_t index) override;

		virtual void onRunRemoved() override;

		virtual void actualize() override;

		virtual ScheduleActualizationAlgorithm* clone(Schedule* schedule, Factory<ScheduleActualizationAlgorithm> *factory);

		static constexpr const char* staticGetName() { return "Routes"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		bool dirty_flag;
		RoutingService* routing_service;
	};
}