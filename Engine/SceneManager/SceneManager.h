#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Attribute.h"
#include "ConceptFactory.h"

#include "Factory.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class Scene;
    class Operation;
    class Order;
    class Vehicle;
    class Performer;
    class Schedule;
	class Run;
	class Stop;
    class ScheduleActualizationAlgorithm;
	class RunVehicleBinder;

	class RoutingService;
	class LoggingService;

    class SCHEDULERENGINE_EXPORT SceneManager
    {
    public:
		SceneManager(RoutingService *routing_service);
        ~SceneManager();

		/// Creates an empty scene
        Scene* createScene();

		/// Destroys the scene recursively destroying all it's content
        void destroyScene(Scene *scene);

		/// Creates or retrieves an existing attribute assignable to vehicles, performers and orders
        const Attribute*createAttribute(const char *name);

    private:
        ConceptFactory<Scene> scenes_factory;
        ConceptFactory<Operation> operations_factory;
        ConceptFactory<Order> orders_factory;
        ConceptFactory<Vehicle> vehicles_factory;
        ConceptFactory<Performer> performers_factory;
        ConceptFactory<Schedule> schedules_factory;
        ConceptFactory<Attribute> attributes_factory;
		ConceptFactory<Run> runs_factory;
		ConceptFactory<Stop> stops_factory;

		Factory<ScheduleActualizationAlgorithm> schedule_actualization_algorithms_factory;
		Factory<RunVehicleBinder> run_vehicle_selectors_factory;

        std::unordered_set<Scene*> scenes;
        std::unordered_map<std::string, Attribute*> attributes;

		RoutingService* routing_service;
    };
}