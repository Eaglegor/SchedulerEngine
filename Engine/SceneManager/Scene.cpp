#include "Scene.h"
#include "Operation.h"
#include "Performer.h"
#include "Run.h"
#include "Schedule.h"
#include "ScheduleActualizationModel.h"
#include "Utils/SceneCloner.h"
#include "Vehicle.h"
#include "WorkStop.h"
#include <Engine/MemoryManager/ObjectSizes.h>
#include <assert.h>

namespace Scheduler
{

	Scene::Scene(size_t id, const SceneContext& context, SceneManager& scene_manager)
	    : id(id),
	      context(context),
	      scene_manager(scene_manager)
	{
	}

	std::size_t Scene::getId( ) const
	{
		return id;
	}

	const Scene::SchedulesList& Scene::getSchedules( ) const
	{
		return schedules;
	}

	Scene::SchedulesList& Scene::getSchedules( )
	{
		return schedules;
	}

	Schedule& Scene::createSchedule(const Performer& performer)
	{
		Schedule::Context context{
		    runs_factory,
		    stops_factory,
		    structural_changes_observer};

		Schedule& schedule = *schedules_factory.createObject(context, performer, *this);

		schedules.push_back(schedule);

		return schedule;
	}

	const SceneContext& Scene::getContext( ) const
	{
		return context;
	}

	SceneManager& Scene::getSceneManager( ) const
	{
		return scene_manager;
	}

	bool Scene::isValid( ) const
	{
		for(Schedule& schedule : schedules)
		{
			if(!schedule.isValid( )) return false;
		}
		return true;
	}

	void Scene::addStructuralChangesListener(StructuralChangesListener& listener)
	{
		structural_changes_observer.addListener(listener);
	}

	void Scene::removeStructuralChangesListener(StructuralChangesListener& listener)
	{
		structural_changes_observer.removeListener(listener);
	}

	Scene::~Scene( )
	{
		for(Schedule& schedule : schedules)
		{
			schedules_factory.destroyObject(&schedule);
		}
	}

	const SceneQueries& Scene::query( ) const
	{
		if(!scene_queries) scene_queries.emplace(const_cast<Scene&>(*this));
		return scene_queries.get( );
	}

	bool Scene::operator==(const Scene& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Scene::operator!=(const Scene& rhs) const
	{
		return !(*this == rhs);
	}
}
