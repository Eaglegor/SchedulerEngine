#include <assert.h>
#include "Scene.h"
#include "Operation.h"
#include "Schedule.h"
#include "Vehicle.h"
#include "Performer.h"
#include "Run.h"
#include "WorkStop.h"
#include "ScheduleActualizationModel.h"
#include "Utils/SceneCloner.h"
#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler {

    Scene::Scene(size_t id, const SceneContext &context):
	id(id),
	context(context),
	scene_manager(nullptr)
	{}

    std::size_t Scene::getId() const {
        return id;
    }
    
	const ImmutableVector<Schedule*>& Scene::getSchedules() const {
        return schedules;
    }

	ImmutableVector<Schedule*>& Scene::getSchedules(){
		return schedules;
	}

    Schedule *Scene::createSchedule(const Performer& performer) {
        Schedule* schedule = schedules_factory.createObject(performer);
		schedule->setScene(this);
		schedule->setRunsFactory(&runs_factory);
		schedule->setStopsFactory(&stops_factory);
		schedule->setStructuralChangesObserver(&structural_changes_observer);

        schedules.push_back(schedule);

        return schedule;
    }
	
	const SceneContext& Scene::getContext() const
	{
		return context;
	}

	SceneManager* Scene::getSceneManager() const
	{
		return scene_manager;
	}
	
	void Scene::setSceneManager(SceneManager* scene_manager)
	{
		this->scene_manager = scene_manager;
	}

	bool Scene::isValid() const
	{
		for(Schedule* schedule : schedules)
		{
			if(!schedule->isValid()) return false;
		}
		return true;
	}

	void Scene::addStructuralChangesListener(StructuralChangesListener* listener)
	{
		structural_changes_observer.addListener(listener);
	}
	
	void Scene::removeStructuralChangesListener(StructuralChangesListener* listener)
	{
		structural_changes_observer.removeListener(listener);
	}
	
    Scene::~Scene() {
        for(Schedule* schedule : schedules)
        {
            schedules_factory.destroyObject(schedule);
        }
    }
    
	SceneQueries& Scene::query()
	{
		if(!scene_queries) scene_queries.emplace(this);
		return scene_queries.get();
	}

}

