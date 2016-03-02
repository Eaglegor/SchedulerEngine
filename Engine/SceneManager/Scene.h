#pragma once

#include <assert.h>
#include <cstdint>
#include <Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/ScheduleActualization/ScheduleActualizationAlgorithm.h>
#include <Engine/SceneManager/RunVehicleBinding/RunVehicleBinder.h>

#include "ConceptFactory.h"
#include "Factory.h"

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

    class Scene
    {
    public:
        Scene(size_t id);
        ~Scene();

        size_t getId() const;

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

		template<typename T, typename... Args>
		bool createRunVehicleBinder(Args &&... args)
		{
			assert(run_vehicle_binders_factory);

			RunVehicleBinder* new_binder = run_vehicle_binders_factory->createObject<T>(this, std::forward<Args>(args)...);

			if(!new_binder) return false;

			if(run_vehicle_binder) run_vehicle_binders_factory->destroyObject(run_vehicle_binder);
			run_vehicle_binder = new_binder;

			return true;
		}

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