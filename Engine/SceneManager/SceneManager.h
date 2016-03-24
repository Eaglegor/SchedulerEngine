#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Attribute.h"
#include "SceneObjectsFactory.h"

#include <Engine/Utils/Factory.h>

#include <SceneManager_export.h>

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

	class RoutingService;
	class LoggingService;

	class ScheduleActualizationAlgorithm;
	class RunVehicleBinder;

    class SCENEMANAGER_EXPORT SceneManager
    {
    public:
		SceneManager(RoutingService *routing_service, MemoryManager* memory_manager);
        ~SceneManager();

		/// Creates an empty scene
        Scene* createScene();

		/// Destroys the scene recursively destroying all it's content
        void destroyScene(Scene *scene);

		/// Creates or retrieves an existing attribute assignable to vehicles, performers and orders
        const Attribute*createAttribute(const char *name);

    private:
		SceneObjectsFactory<Scene> scenes_factory;
		SceneObjectsFactory<Operation> operations_factory;
		SceneObjectsFactory<Order> orders_factory;
		SceneObjectsFactory<Vehicle> vehicles_factory;
		SceneObjectsFactory<Performer> performers_factory;
		SceneObjectsFactory<Schedule> schedules_factory;
		SceneObjectsFactory<Attribute> attributes_factory;
		SceneObjectsFactory<Run> runs_factory;
		SceneObjectsFactory<Stop> stops_factory;

		Factory<ScheduleActualizationAlgorithm> schedule_actualization_algorithms_factory;
		Factory<RunVehicleBinder> run_vehicle_selectors_factory;

        std::unordered_set<Scene*> scenes;
        std::unordered_map<std::string, Attribute*> attributes;

		RoutingService* routing_service;
    };
}