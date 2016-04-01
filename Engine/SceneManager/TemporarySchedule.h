#pragma once

#include <Engine/SceneManager/SceneObjectsFactory.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class Schedule;

	class SCENEMANAGER_EXPORT TemporarySchedule
	{
	public:
		TemporarySchedule(Schedule* schedule, SceneObjectsFactory<Schedule>* schedules_factory);
		~TemporarySchedule();

		Schedule* operator->();
		const Schedule* operator->() const;

		Schedule* get();
		const Schedule* get() const;
		
	private:
		Schedule* schedule;
		SceneObjectsFactory<Schedule> *schedules_factory;
	};
}