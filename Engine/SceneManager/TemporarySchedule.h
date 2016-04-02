#pragma once

#include <Engine/SceneManager/SceneObjectsFactory.h>
#include <memory>
#include <SceneManager_export.h>

namespace Scheduler
{
	class Schedule;

	class SCENEMANAGER_EXPORT TemporaryScheduleDeleter
	{
	public:
		TemporaryScheduleDeleter(SceneObjectsFactory<Schedule>* factory):scene_objects_factory(factory){}

		void operator()(Schedule* schedule) const;

	private:
		SceneObjectsFactory<Schedule>* scene_objects_factory;
	};

	using TemporarySchedule = std::unique_ptr<Schedule, TemporaryScheduleDeleter>;
	
}