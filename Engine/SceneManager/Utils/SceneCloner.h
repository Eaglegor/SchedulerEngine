#pragma once


namespace Scheduler
{
	class Scene;
	class Schedule;

	class SceneCloner
	{
	public:
		static void cloneScheduleProperties(const Schedule& from, Schedule& to);
		static void cloneScheduleAssignments(const Schedule& from, Schedule& to);
		static void cloneScheduleState(const Schedule& from, Schedule& to);

		static void cloneSceneSchedulesPool(const Scene& from, Scene& to);
		static void cloneSceneAssignments(const Scene& from, Scene& to);
	};
}
