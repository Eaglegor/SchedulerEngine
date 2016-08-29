#include "SceneManager.h"

#include "SceneContext.h"
#include "Scene.h"
#include "Operation.h"
#include "Order.h"
#include "Vehicle.h"
#include "Performer.h"
#include "Schedule.h"
#include "Run.h"
#include "WorkStop.h"

#include "ScheduleActualizationModel.h"
#include "ScheduleValidationModel.h"
#include "Extensions/RouteActualizationAlgorithm.h"
#include "Extensions/ArrivalTimeActualizationAlgorithm.h"
#include "Extensions/DurationActualizationAlgorithm.h"
#include "Extensions/RunVehicleBinder.h"
#include "Extensions/StopValidationAlgorithm.h"
#include "Extensions/ScheduleValidationAlgorithm.h"
#include "Extensions/RunValidationAlgorithm.h"

#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler
{
	SceneManager::SceneManager() :
		schedule_actualization_models_factory(Pool::MEDIUM_OBJECT),
		route_actualization_algorithms_factory(Pool::MEDIUM_OBJECT),
		duration_actualization_algorithms_factory(Pool::MEDIUM_OBJECT),
		arrival_time_actualization_algorithms_factory(Pool::MEDIUM_OBJECT),
		schedule_validation_models_factory(Pool::MEDIUM_OBJECT),
		schedule_validation_algorithms_factory(Pool::MEDIUM_OBJECT),
		run_validation_algorithms_factory(Pool::MEDIUM_OBJECT),
		stop_validation_algorithms_factory(Pool::MEDIUM_OBJECT),
		run_vehicle_binders_factory(Pool::MEDIUM_OBJECT)
	{
	}
	
	SceneContext* SceneManager::createSceneContext()
	{
		SceneContext* scene_context = scene_contexts_factory.createObject();
		scene_contexts.insert(scene_context);
		return scene_context;
	}
	
	void SceneManager::destroySceneContext(SceneContext* scene_context)
	{
		scene_contexts.erase(scene_context);
		scene_contexts_factory.destroyObject(scene_context);
	}

	Scene* SceneManager::createScene(const SceneContext& context)
	{
		Scene *scene = scenes_factory.createObject(context);
		scene->setSceneManager(this);
		scenes.insert(scene);
		return scene;
	}

	ScheduleActualizationModel* SceneManager::createScheduleActualizationModel()
	{
		ScheduleActualizationModel* model = schedule_actualization_models_factory.createObject<ScheduleActualizationModel>();
		schedule_actualization_models.emplace(model);
		return model;
	}

	void SceneManager::destroyScheduleActualizationModel(ScheduleActualizationModel* model)
	{
		assert(schedule_actualization_models.find(model) != schedule_actualization_models.end());
		schedule_actualization_models.erase(model);
		schedule_actualization_models_factory.destroyObject(model);
	}

	ScheduleValidationModel* SceneManager::createScheduleValidationModel()
	{
		ScheduleValidationModel* model = schedule_validation_models_factory.createObject<ScheduleValidationModel>();
		schedule_validation_models.emplace(model);
		return model;
	}

	void SceneManager::destroyScheduleValidationModel(ScheduleValidationModel* model)
	{
		assert(schedule_validation_models.find(model) != schedule_validation_models.end());
		schedule_validation_models.erase(model);
		schedule_validation_models_factory.destroyObject(model);
	}

	void SceneManager::destroyRouteActualizationAlgorithm(RouteActualizationAlgorithm* algorithm)
	{
		assert(route_actualization_algorithms.find(algorithm) != route_actualization_algorithms.end());
		route_actualization_algorithms.erase(algorithm);
		route_actualization_algorithms_factory.destroyObject(algorithm);
	}

	void SceneManager::destroyArrivalTimeActualizationAlgorithm(ArrivalTimeActualizationAlgorithm* algorithm)
	{
		assert(arrival_time_actualization_algorithms.find(algorithm) != arrival_time_actualization_algorithms.end());
		arrival_time_actualization_algorithms.erase(algorithm);
		arrival_time_actualization_algorithms_factory.destroyObject(algorithm);
	}

	void SceneManager::destroyDurationActualizationAlgorithm(DurationActualizationAlgorithm* algorithm)
	{
		assert(duration_actualization_algorithms.find(algorithm) != duration_actualization_algorithms.end());
		duration_actualization_algorithms.erase(algorithm);
		duration_actualization_algorithms_factory.destroyObject(algorithm);
	}

	void SceneManager::destroyScheduleValidationAlgorithm(ScheduleValidationAlgorithm* algorithm)
	{
		assert(schedule_validation_algorithms.find(algorithm) != schedule_validation_algorithms.end());
		schedule_validation_algorithms.erase(algorithm);
		schedule_validation_algorithms_factory.destroyObject(algorithm);
	}

	void SceneManager::destroyRunValidationAlgorithm(RunValidationAlgorithm* algorithm)
	{
		assert(run_validation_algorithms.find(algorithm) != run_validation_algorithms.end());
		run_validation_algorithms.erase(algorithm);
		run_validation_algorithms_factory.destroyObject(algorithm);
	}

	void SceneManager::destroyStopValidationAlgorithm(StopValidationAlgorithm* algorithm)
	{
		assert(stop_validation_algorithms.find(algorithm) != stop_validation_algorithms.end());
		stop_validation_algorithms.erase(algorithm);
		stop_validation_algorithms_factory.destroyObject(algorithm);
	}

	void SceneManager::destroyRunVehicleBinder(RunVehicleBinder* binder)
	{
		assert(run_vehicle_binders.find(binder) != run_vehicle_binders.end());
		run_vehicle_binders.erase(binder);
		run_vehicle_binders_factory.destroyObject(binder);
	}
	
	void SceneManager::destroyScene(Scene *scene)
	{
		scenes.erase(scene);
		scenes_factory.destroyObject(scene);
	}

	SceneManager::~SceneManager()
	{
		for (Scene *scene: scenes) 
		{
			scenes_factory.destroyObject(scene);
		}

		for(ScheduleActualizationModel* actualization_model : schedule_actualization_models)
		{
			schedule_actualization_models_factory.destroyObject(actualization_model);
		}

		for (RouteActualizationAlgorithm* algorithm : route_actualization_algorithms)
		{
			route_actualization_algorithms_factory.destroyObject(algorithm);
		}

		for (ArrivalTimeActualizationAlgorithm* algorithm : arrival_time_actualization_algorithms)
		{
			arrival_time_actualization_algorithms_factory.destroyObject(algorithm);
		}

		for (DurationActualizationAlgorithm* algorithm : duration_actualization_algorithms)
		{
			duration_actualization_algorithms_factory.destroyObject(algorithm);
		}

		for (ScheduleValidationModel* validation_model: schedule_validation_models)
		{
			schedule_validation_models_factory.destroyObject(validation_model);
		}

		for (ScheduleValidationAlgorithm* algorithm : schedule_validation_algorithms)
		{
			schedule_validation_algorithms_factory.destroyObject(algorithm);
		}

		for (RunValidationAlgorithm* algorithm : run_validation_algorithms)
		{
			run_validation_algorithms_factory.destroyObject(algorithm);
		}

		for (StopValidationAlgorithm* algorithm : stop_validation_algorithms)
		{
			stop_validation_algorithms_factory.destroyObject(algorithm);
		}

		for (RunVehicleBinder* binder : run_vehicle_binders)
		{
			run_vehicle_binders_factory.destroyObject(binder);
		}
	}
}