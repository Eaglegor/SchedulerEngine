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
	class SceneContext;

    class SCENEMANAGER_EXPORT SceneManager
    {
    public:
		SceneManager();
        ~SceneManager();

		SceneContext* createSceneContext();
		
		void destroySceneContext(SceneContext* scene_context);
		
		/// Creates an empty scene
        Scene* createScene(const SceneContext& context);

		/// Destroys the scene recursively destroying all it's content
        void destroyScene(Scene *scene);

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

		template<typename T, typename... Args>
		T* createRunVehicleBinder(Args &&... args)
		{
			T* new_binder = run_vehicle_binders_factory.createObject<T>(std::forward<Args>(args)...);

			if (!new_binder) return nullptr;

			run_vehicle_binders.emplace(new_binder);

			return new_binder;
		}
		
		void destroyRunVehicleBinder(RunVehicleBinder* binder);
		
    private:
		SceneObjectsFactory<Scene> scenes_factory;
		SceneObjectsFactory<SceneContext> scene_contexts_factory;

		Factory<ScheduleActualizationModel> schedule_actualization_models_factory;
		Factory<RouteActualizationAlgorithm> route_actualization_algorithms_factory;
		Factory<ArrivalTimeActualizationAlgorithm> arrival_time_actualization_algorithms_factory;
		Factory<DurationActualizationAlgorithm> duration_actualization_algorithms_factory;

		Factory<ScheduleValidationModel> schedule_validation_models_factory;
		Factory<ScheduleValidationAlgorithm> schedule_validation_algorithms_factory;
		Factory<RunValidationAlgorithm> run_validation_algorithms_factory;
		Factory<StopValidationAlgorithm> stop_validation_algorithms_factory;

		Factory<RunVehicleBinder> run_vehicle_binders_factory;

		std::unordered_set<ScheduleActualizationModel*> schedule_actualization_models;
		std::unordered_set<RouteActualizationAlgorithm*> route_actualization_algorithms;
		std::unordered_set<ArrivalTimeActualizationAlgorithm*> arrival_time_actualization_algorithms;
		std::unordered_set<DurationActualizationAlgorithm*> duration_actualization_algorithms;

		std::unordered_set<ScheduleValidationModel*> schedule_validation_models;
		std::unordered_set<ScheduleValidationAlgorithm*> schedule_validation_algorithms;
		std::unordered_set<RunValidationAlgorithm*> run_validation_algorithms;
		std::unordered_set<StopValidationAlgorithm*> stop_validation_algorithms;
		
		std::unordered_set<RunVehicleBinder*> run_vehicle_binders;

		std::unordered_set<SceneContext*> scene_contexts;
        std::unordered_set<Scene*> scenes;
    };
}