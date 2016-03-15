#pragma once

#include <assert.h>
#include <cstddef>
#include <Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/ScheduleActualization/ScheduleActualizationAlgorithm.h>
#include <Engine/SceneManager/RunVehicleBinding/RunVehicleBinder.h>

#include "ConceptFactory.h"
#include "Factory.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Operation;
    class Order;
    class Performer;
    class Vehicle;
    class Attribute;
    class Schedule;
	class Run;
	class Stop;
	class LoggingService;

	class RoutingService;

	class ScheduleActualizersFactory;
	class RunVehicleBinder;

    class SCHEDULERENGINE_EXPORT Scene
    {
    public:
        Scene(size_t id);
        ~Scene();

        size_t getId() const;

		/**
		  Free operations are not bound to the orders and may have multiple instances on the scene. 
		  They represent the mandatory service operations which must precede or follow some other operations 
		  (e.g. visiting a veterinary station before delivering meat or visiting a bank to unload all the collected cash)
		*/
		const ImmutableVector<Operation*>& getFreeOperations() const;
		const ImmutableVector<Order*>& getOrders() const;
		const ImmutableVector<Schedule*>& getSchedules() const;
		const ImmutableVector<Performer*>& getPerformers() const;
		const ImmutableVector<Vehicle*>& getVehicles() const;

        ImmutableVector<Operation*>& getFreeOperations();
        ImmutableVector<Order*>& getOrders();
        ImmutableVector<Schedule*>& getSchedules();
		ImmutableVector<Performer*>& getPerformers();
		ImmutableVector<Vehicle*>& getVehicles();

        Operation* createFreeOperation();
        Order* createOrder();
        Schedule* createSchedule(const Performer *performer);
		Performer* createPerformer();
		Vehicle* createVehicle();

		template<typename T, typename... Args>
		T* createRunVehicleBinder(Args &&... args)
		{
			assert(run_vehicle_binders_factory);

			T* new_binder = run_vehicle_binders_factory->createObject<T>(this, std::forward<Args>(args)...);

			if (!new_binder) return nullptr;

			if (run_vehicle_binder) run_vehicle_binders_factory->destroyObject(run_vehicle_binder);
			run_vehicle_binder = new_binder;

			return new_binder;
		}

		// == framework internal ====================================
        void setOperationsfactory(ConceptFactory<Operation> *factory);
        void setOrdersFactory(ConceptFactory<Order> *factory);
        void setPerformersFactory(ConceptFactory<Performer> *factory);
        void setVehiclesFactory(ConceptFactory<Vehicle> *factory);
        void setSchedulesFactory(ConceptFactory<Schedule> *factory);
		void setRunsFactory(ConceptFactory<Run> *factory);
		void setStopsFactory(ConceptFactory<Stop> *factory);

		void setRoutingService(RoutingService *routing_service);

		void setScheduleActualizationAlgorithmsFactory(Factory<ScheduleActualizationAlgorithm>* factory);

		void setRunVehicleSelectorsFactory(Factory<RunVehicleBinder>* factory);

    private:
        size_t id;

        std::vector<Operation*> free_operations;
        std::vector<Order*> orders;
        std::vector<Schedule*> schedules;

		std::vector<Performer*> performers;
		std::vector<Vehicle*> vehicles;

        ConceptFactory<Operation> *operations_factory;
        ConceptFactory<Order> *orders_factory;
        ConceptFactory<Performer> *performers_factory;
        ConceptFactory<Vehicle> *vehicles_factory;
        ConceptFactory<Schedule> *schedules_factory;

		ConceptFactory<Run> *runs_factory;
		ConceptFactory<Stop> *stops_factory;

		Factory<ScheduleActualizationAlgorithm> *schedule_actualization_algorithms_factory;
		Factory<RunVehicleBinder> *run_vehicle_binders_factory;

		RunVehicleBinder *run_vehicle_binder;

		RoutingService* routing_service;
    };
}