#include "ScheduleVariant.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Schedule.h"
#include "Utils/SceneCloner.h"
#include <assert.h>

namespace Scheduler
{
	ScheduleVariant::ScheduleVariant( )
	{
	}

	ScheduleVariant::ScheduleVariant(Schedule& schedule)
	    : original_schedule(schedule)
	{
		SceneManager& scene_manager = original_schedule->getScene( ).getSceneManager( );
		new_scene                   = scene_manager.createScene(original_schedule->getScene( ).getContext( ));
		new_schedule                = new_scene->createSchedule(original_schedule->getPerformer( ));
		SceneCloner::cloneScheduleState(original_schedule.get( ), new_schedule.get( ));
	}

	ScheduleVariant::ScheduleVariant(ScheduleVariant&& rhs)
	    : original_schedule(rhs.original_schedule),
	      new_schedule(rhs.new_schedule),
	      new_scene(rhs.new_scene)
	{
		rhs.original_schedule = None;
		rhs.new_schedule      = None;
		rhs.new_scene         = None;
	}

	ScheduleVariant::~ScheduleVariant( )
	{
		if(new_schedule)
		{
			SceneManager& scene_manager = new_scene->getSceneManager( );
			scene_manager.destroyScene(new_scene.get( ));
		}
	}

	ScheduleVariant& ScheduleVariant::operator=(ScheduleVariant&& rhs)
	{
		this->original_schedule = rhs.original_schedule;
		this->new_schedule      = rhs.new_schedule;
		this->new_scene         = rhs.new_scene;

		rhs.original_schedule = None;
		rhs.new_schedule      = None;
		rhs.new_scene         = None;

		return *this;
	}

	ScheduleVariant& ScheduleVariant::operator=(Schedule& rhs)
	{
		if(new_schedule)
		{
			SceneManager& scene_manager = new_scene->getSceneManager( );
			scene_manager.destroyScene(new_scene.get( ));
		}
		this->original_schedule = rhs;

		SceneManager& scene_manager = original_schedule->getScene( ).getSceneManager( );
		new_scene                   = scene_manager.createScene(original_schedule->getScene( ).getContext( ));
		new_schedule                = new_scene->createSchedule(original_schedule->getPerformer( ));
		SceneCloner::cloneScheduleState(original_schedule.get( ), new_schedule.get( ));

		return *this;
	}

	void ScheduleVariant::apply( ) const
	{
		assert(new_schedule);
		if(!new_schedule) return;
		SceneCloner::cloneScheduleState(new_schedule.get( ), original_schedule.get( ));
	}

	Optional<Schedule&> ScheduleVariant::getSchedule( ) const
	{
		return new_schedule;
	}

	ScheduleVariant::operator bool( ) const
	{
		return new_schedule ? true : false;
	}
}