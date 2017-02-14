#pragma once
#include <SceneManager_export.h>

namespace Scheduler
{
	class Scene;
	class Schedule;
	
	class SCENEMANAGER_EXPORT SceneCloner
	{
	public:
		static void cloneScheduleProperties(const Schedule& from, Schedule& to);
		static void cloneScheduleAssignments(const Schedule& from, Schedule& to);
		static void cloneScheduleState(const Schedule& from, Schedule& to);
		
		static void cloneSceneSchedulesPool(const Scene& from, Scene& to);
		static void cloneSceneAssignments(const Scene& from, Scene& to);
	};
}
