#include "ScheduleVariant.h"
#include "Schedule.h"
#include "Utils/SceneCloner.h"
#include "Scene.h"
#include "SceneManager.h"

namespace Scheduler
{
	ScheduleVariant::ScheduleVariant():
	original_schedule(nullptr),
	new_schedule(nullptr)
	{}
	
	ScheduleVariant::ScheduleVariant(Schedule* schedule):
	original_schedule(schedule)
	{
		SceneManager* scene_manager = original_schedule->getScene()->getSceneManager();
		Scene* scene = scene_manager->createScene(original_schedule->getScene()->getContext());
		new_schedule = scene->createSchedule(*original_schedule->getPerformer());
		SceneCloner::cloneScheduleState(original_schedule, new_schedule);
	}
	
	ScheduleVariant::ScheduleVariant(ScheduleVariant&& rhs):
	original_schedule(rhs.original_schedule),
	new_schedule(rhs.new_schedule)
	{
		rhs.original_schedule = nullptr;
		rhs.new_schedule = nullptr;
	}
	
	ScheduleVariant::~ScheduleVariant()
	{
		if(new_schedule != nullptr)
		{
			SceneManager* scene_manager = new_schedule->getScene()->getSceneManager();
			scene_manager->destroyScene(new_schedule->getScene());
		}
	}

	ScheduleVariant& ScheduleVariant::operator=(ScheduleVariant&& rhs)
	{
		this->original_schedule = rhs.original_schedule;
		this->new_schedule = rhs.new_schedule;
		
		rhs.original_schedule = nullptr;
		rhs.new_schedule = nullptr;
	}
	
	void ScheduleVariant::apply() const
	{
		if(new_schedule == nullptr) return;
		SceneCloner::cloneScheduleState(new_schedule, original_schedule);
	}
	
	Schedule* ScheduleVariant::getSchedule() const
	{
		return new_schedule;
	}
	
	ScheduleVariant::operator bool() const
	{
		return new_schedule != nullptr;
	}

}