#include "SceneManager.h"

#include "Scene.h"
#include "Operation.h"
#include "Order.h"
#include "Vehicle.h"
#include "Performer.h"
#include "Schedule.h"
#include "Run.h"
#include "WorkStop.h"

#include "ScheduleActualizationModel.h"
#include "Extensions/RouteActualizationAlgorithm.h"
#include "Extensions/ArrivalTimeActualizationAlgorithm.h"
#include "Extensions/DurationActualizationAlgorithm.h"
#include "Extensions/RunVehicleBinder.h"

#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler
{
	SceneManager::SceneManager(RoutingService* routing_service, MemoryManager* memory_manager) :
			scenes_factory(memory_manager, 5),
			operations_factory(memory_manager),
			orders_factory(memory_manager),
			vehicles_factory(memory_manager),
			performers_factory(memory_manager),
			schedules_factory(memory_manager),
			attributes_factory(memory_manager),
			runs_factory(memory_manager),
			stops_factory(memory_manager),
			schedule_actualization_models_factory(memory_manager, Pool::MEDIUM_OBJECT, 5),
			route_actualization_algorithms_factory(memory_manager, Pool::MEDIUM_OBJECT, 5),
			duration_actualization_algorithms_factory(memory_manager, Pool::MEDIUM_OBJECT, 5),
			arrival_time_actualization_algorithms_factory(memory_manager, Pool::MEDIUM_OBJECT, 5),
			run_vehicle_selectors_factory(memory_manager, Pool::MEDIUM_OBJECT, 100),
			routing_service(routing_service)
	{
	}

	Scene* SceneManager::createScene()
	{
		Scene *scene = scenes_factory.createObject();

		scene->setOperationsfactory(&operations_factory);
		scene->setOrdersFactory(&orders_factory);
		scene->setPerformersFactory(&performers_factory);
		scene->setSchedulesFactory(&schedules_factory);
		scene->setVehiclesFactory(&vehicles_factory);
		scene->setRunsFactory(&runs_factory);
		scene->setStopsFactory(&stops_factory);

		scene->setRunVehicleSelectorsFactory(&run_vehicle_selectors_factory);

		scene->setRoutingService(routing_service);

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

	void SceneManager::destroyScene(Scene *scene)
	{
		scenes.erase(scene);
		scenes_factory.destroyObject(scene);
	}

	const Attribute* SceneManager::createAttribute(const char *name)
	{
		auto iter = attributes.find(name);
		if (iter != attributes.end()) return iter->second;

		Attribute* attribute = attributes_factory.createObject(name);
		attributes.emplace(name, attribute);

		return attribute;
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

		for (auto &iter : attributes)
		{
			attributes_factory.destroyObject(iter.second);
		}
	}
}