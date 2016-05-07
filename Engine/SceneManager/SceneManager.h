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
	class WorkStop;

	class RoutingService;
	class LoggingService;

	class ScheduleActualizationModel;
	class RouteActualizationAlgorithm;
	class ArrivalTimeActualizationAlgorithm;
	class DurationActualizationAlgorithm;
	class RunVehicleBinder;

	class ScheduleValidationModel;
	class ScheduleValidationAlgorithm;
	class RunValidationAlgorithm;
	class StopValidationAlgorithm;

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

		ScheduleActualizationModel* createScheduleActualizationModel();
		void destroyScheduleActualizationModel(ScheduleActualizationModel* model);

		ScheduleValidationModel* createScheduleValidationModel();
		void destroyScheduleValidationModel(ScheduleValidationModel* model);

		template<typename T, typename... Args>
		T* createRouteActualizationAlgorithm(Args&& ...args)
		{
			static_assert(std::is_base_of<RouteActualizationAlgorithm, T>::value, "Incompatible class");
			T* algorithm = route_actualization_algorithms_factory.createObject<T>(std::forward<Args>(args)...);
			route_actualization_algorithms.emplace(algorithm);
			return algorithm;
		}

		void destroyRouteActualizationAlgorithm(RouteActualizationAlgorithm* algorithm);

		template<typename T, typename... Args>
		T* createArrivalTimeActualizationAlgorithm(Args&& ...args)
		{
			static_assert(std::is_base_of<ArrivalTimeActualizationAlgorithm, T>::value, "Incompatible class");
			T* algorithm = arrival_time_actualization_algorithms_factory.createObject<T>(std::forward<Args>(args)...);
			arrival_time_actualization_algorithms.emplace(algorithm);
			return algorithm;
		}

		void destroyArrivalTimeActualizationAlgorithm(ArrivalTimeActualizationAlgorithm* algorithm);

		template<typename T, typename... Args>
		T* createDurationActualizationAlgorithm(Args&& ...args)
		{
			static_assert(std::is_base_of<DurationActualizationAlgorithm, T>::value, "Incompatible class");
			T* algorithm = duration_actualization_algorithms_factory.createObject<T>(std::forward<Args>(args)...);
			duration_actualization_algorithms.emplace(algorithm);
			return algorithm;
		}

		void destroyDurationActualizationAlgorithm(DurationActualizationAlgorithm* algorithm);

		template<typename T, typename... Args>
		T* createScheduleValidationAlgorithm(Args&& ...args)
		{
			static_assert(std::is_base_of<ScheduleValidationAlgorithm, T>::value, "Incompatible class");
			T* algorithm = schedule_validation_algorithms_factory.createObject<T>(std::forward<Args>(args)...);
			schedule_validation_algorithms.emplace(algorithm);
			return algorithm;
		}

		void destroyScheduleValidationAlgorithm(ScheduleValidationAlgorithm* algorithm);

		template<typename T, typename... Args>
		T* createRunValidationAlgorithm(Args&& ...args)
		{
			static_assert(std::is_base_of<RunValidationAlgorithm, T>::value, "Incompatible class");
			T* algorithm = run_validation_algorithms_factory.createObject<T>(std::forward<Args>(args)...);
			run_validation_algorithms.emplace(algorithm);
			return algorithm;
		}

		void destroyRunValidationAlgorithm(RunValidationAlgorithm* algorithm);

		template<typename T, typename... Args>
		T* createStopValidationAlgorithm(Args&& ...args)
		{
			static_assert(std::is_base_of<StopValidationAlgorithm, T>::value, "Incompatible class");
			T* algorithm = stop_validation_algorithms_factory.createObject<T>(std::forward<Args>(args)...);
			stop_validation_algorithms.emplace(algorithm);
			return algorithm;
		}

		void destroyStopValidationAlgorithm(StopValidationAlgorithm* algorithm);

    private:
		SceneObjectsFactory<Scene> scenes_factory;
		SceneObjectsFactory<Operation> operations_factory;
		SceneObjectsFactory<Order> orders_factory;
		SceneObjectsFactory<Vehicle> vehicles_factory;
		SceneObjectsFactory<Performer> performers_factory;
		SceneObjectsFactory<Schedule> schedules_factory;
		SceneObjectsFactory<Attribute> attributes_factory;
		SceneObjectsFactory<Run> runs_factory;
		SceneObjectsFactory<WorkStop> stops_factory;

		Factory<ScheduleActualizationModel> schedule_actualization_models_factory;
		Factory<RouteActualizationAlgorithm> route_actualization_algorithms_factory;
		Factory<ArrivalTimeActualizationAlgorithm> arrival_time_actualization_algorithms_factory;
		Factory<DurationActualizationAlgorithm> duration_actualization_algorithms_factory;

		Factory<ScheduleValidationModel> schedule_validation_models_factory;
		Factory<ScheduleValidationAlgorithm> schedule_validation_algorithms_factory;
		Factory<RunValidationAlgorithm> run_validation_algorithms_factory;
		Factory<StopValidationAlgorithm> stop_validation_algorithms_factory;

		Factory<RunVehicleBinder> run_vehicle_selectors_factory;

		std::unordered_set<ScheduleActualizationModel*> schedule_actualization_models;
		std::unordered_set<RouteActualizationAlgorithm*> route_actualization_algorithms;
		std::unordered_set<ArrivalTimeActualizationAlgorithm*> arrival_time_actualization_algorithms;
		std::unordered_set<DurationActualizationAlgorithm*> duration_actualization_algorithms;

		std::unordered_set<ScheduleValidationModel*> schedule_validation_models;
		std::unordered_set<ScheduleValidationAlgorithm*> schedule_validation_algorithms;
		std::unordered_set<RunValidationAlgorithm*> run_validation_algorithms;
		std::unordered_set<StopValidationAlgorithm*> stop_validation_algorithms;

        std::unordered_set<Scene*> scenes;
        std::unordered_map<std::string, Attribute*> attributes;

		RoutingService* routing_service;
    };
}