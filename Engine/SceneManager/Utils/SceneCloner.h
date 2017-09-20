#pragma once


namespace Scheduler
{
	class Scene;
	class Schedule;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Utilitary class used to copy the state from one scene to another
	 * 
	 * @note Scenes have to share the same scene context
	 */
	class SceneCloner
	{
	public:
		/**
		 * @brief Copies the settings from one schedule to another
		 * 
		 * @detail This copies the schedule settings (work shift time, actualization/validation models, constraints) from one schedule to
		 * another. This doesn't copy the order assignments.
		 * 
		 * @sa SceneCloner::cloneScheduleAssignments
		 * 
		 * @param from Schedule to copy the settings from
		 * @param to Schedule to copy the settings to
		 */
		static void cloneScheduleProperties(const Schedule& from, Schedule& to);

		/**
		 * @brief Copies the order assignments from one schedule to another
		 * 
		 * @detail This copies the order assignments (runs, stops) from one schedule to
		 * another. This doesn't copy the order assignments. The sequence order of the runs and stops is preserved.
		 * If there were any stops assigned to the @c "to" schedule, they are destroyed.
		 * 
		 * @sa SceneCloner::cloneScheduleProperties
		 * 
		 * @param from Schedule to copy the settings from
		 * @param to Schedule to copy the settings to
		 */
		static void cloneScheduleAssignments(const Schedule& from, Schedule& to);

		/**
		 * @brief Copies the settings and assignments from one schedule to another
		 * 
		 * @sa SceneCloner::cloneScheduleProperties, SceneCloner::cloneScheduleAssignments
		 * 
		 * @param from Schedule to copy the settings from
		 * @param to Schedule to copy the settings to
		 */
		static void cloneScheduleState(const Schedule& from, Schedule& to);

		/**
		 * @brief Copies the hollow schedules from one scene to another
		 * 
		 * @details This copies the hollow copies of the schedules from one scene to another.
		 * The order of schedules is preserved, i.e. the iteration over the list of schedules in the @c "to" scene will process
		 * the copies in the same order as their corresponding originals were processed in the @c "from" scene
		 * No assignments are copied so all schedules are empty. If there were any schedules in the @c "to" scene, they are destoyed.
		 * 
		 * @sa SceneCloner::cloneSceneAssignments
		 * 
		 * @param from Schedule to copy the settings from
		 * @param to Schedule to copy the settings to
		 */
		static void cloneSceneSchedulesPool(const Scene& from, Scene& to);

		/**
		 * @brief Copies the assignments from one scene to another
		 * 
		 * @details This copies the assignments (runs, stops) from one scene to another.
		 * If there were any stops in the @c "to" scene, they are destoyed.
		 * 
		 * @warning This method assumes that the order of schedules in the @c "from" scene matches the order of schedules in the 
		 * @c "to" scene. It iterates over the schedules of the @c "from" scene and the schedules of the @c "to" scene and copies the 
		 * assignments using the SceneCloner::cloneScheduleAssignments method. The state of first schedule of @c "from" is copied to the
		 * first schedule of @c "to", of the second - to the second schedule of @c "to" etc. If the schedules list of @c "from" and @c "to" scenes don't
		 * match each other, the behavior is undefined.
		 * 
		 * @sa SceneCloner::cloneSceneSchedulesPool
		 * 
		 * @param from Schedule to copy the settings from
		 * @param to Schedule to copy the settings to
		 */
		static void cloneSceneAssignments(const Scene& from, Scene& to);
	};
}
