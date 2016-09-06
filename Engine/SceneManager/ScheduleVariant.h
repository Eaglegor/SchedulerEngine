#pragma once

#include <SceneManager_export.h>

namespace Scheduler
{
	class Schedule;
	
	class SCENEMANAGER_EXPORT ScheduleVariant
	{
	public:
		ScheduleVariant();
		ScheduleVariant(Schedule* schedule);
		ScheduleVariant(ScheduleVariant&& rhs);
		~ScheduleVariant();
		
		ScheduleVariant& operator=(ScheduleVariant&& rhs);
		
		Schedule* getSchedule() const;
		void apply() const;
		
		operator bool() const;
		
	private:
		Schedule* original_schedule;
		Schedule* new_schedule;
	};
}