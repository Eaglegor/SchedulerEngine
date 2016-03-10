#include "SceneManager.h"
#include "Scene.h"

namespace Scheduler
{
	SceneManager::SceneManager(RoutingService* routing_service) :
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