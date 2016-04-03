#pragma once

#include <SceneManager_export.h>

namespace Scheduler
{
	class Schedule;

	class SCENEMANAGER_EXPORT ScheduleStateUtils
	{
	public:
		static void copyState(const Schedule* from, Schedule* to);
	};
}