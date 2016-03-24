#include "SceneManager.h"

#include "Scene.h"
#include "Operation.h"
#include "Order.h"
#include "Vehicle.h"
#include "Performer.h"
#include "Schedule.h"
#include "Run.h"
#include "Stop.h"

#include "Extensions/ScheduleActualizationAlgorithm.h"
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
			schedule_actualization_algorithms_factory(memory_manager, Pool::MEDIUM_OBJECT, 100),
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

		scene->setScheduleActualizationAlgorithmsFactory(&schedule_actualization_algorithms_factory);
		scene->setRunVehicleSelectorsFactory(&run_vehicle_selectors_factory);

		scene->setRoutingService(routing_service);

		scenes.insert(scene);
		return scene;
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
		for (Scene *scene: scenes) {
			scenes_factory.destroyObject(scene);
		}
	}
}