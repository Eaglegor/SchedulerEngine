#pragma once
#include <SceneManager_export.h>

namespace Scheduler
{
	class Scene;
	class Schedule;
	
	class SCENEMANAGER_EXPORT SceneCloner
	{
	public:
		static void cloneScheduleState(const Schedule& from, Schedule& to);
	};
}
