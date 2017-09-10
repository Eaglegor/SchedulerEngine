#pragma once

#include <Engine/Utils/Optional.h>


namespace Scheduler
{
	class Schedule;
	class Scene;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Temporary copy of the schedule
	 * 
	 * @details Scene manager API is designed in such a way that the schedule must 
	 * always be attached to the scene and there can be only a single copy of a schedule.
	 * 
	 * In some cases we still want to have multiple copies of a single schedule e.g.
	 * when we want to make some different changes on each copy of the schedule (probably concurrently)
	 * and then select the variant having the best cost.
	 * 
	 * In this case we have to make a copy of the whole scene containing the schedule so that
	 * the internal caches of scene queries or actualization state from different variants
	 * don't mix up. 
	 * 
	 * ScheduleVariant hides this copying behind the constructor/assignment operator to
	 * simplify algorithmic code.
	 */
	class ScheduleVariant
	{
	public:
		/**
		 * @brief Default constructor
		 * 
		 * @details Makes a schedule variant that doesn't represent a copy of any schedule
		 */
		ScheduleVariant( );

		/**
		 * @brief Schedule-based constructor
		 * 
		 * @details Makes a schedule variant representing a copy of schedule
		 * 
		 * @param schedule Schedule to make a copy from
		 */
		ScheduleVariant(Schedule& schedule);

		/**
		 * @brief Move constructor
		 * 
		 * @details Transfers the state from another schedule variant
		 * 
		 * @param rhs Another schedule variant
		 */
		ScheduleVariant(ScheduleVariant&& rhs);

		/**
		 * @brief Destructor
		 * 
		 * @details If this variant represents a copy of some schedule, this copy is destroyed
		 * along with it's temporary scene
		 */
		~ScheduleVariant( );

		/**
		 * @brief Move assignment operator
		 * 
		 * @details Transfers the state from another schedule variant
		 * 
		 * @param rhs Another schedule variant
		 * 
		 * @return Reference to @c this
		 */
		ScheduleVariant& operator=(ScheduleVariant&& rhs);

		/**
		 * @brief Assignment operator
		 * 
		 * @details Creates the copy of the scene containing @c rhs and a copy of @c rhs in the new scene.
		 * Takes ownership on created scene copy. If there already was a schedule copy associated with this variant
		 * it's destroyed.
		 * 
		 * @param rhs Schedule to copy the state from
		 * 
		 * @return Reference to @c this
		 */
		ScheduleVariant& operator=(Schedule& rhs);

		/**
		 * @brief Returns the stored schedule copy if any
		 * 
		 * @return Reference to @c the stored schedule or Scheduler::None if there is no schedule stored
		 */
		Optional<Schedule&> getSchedule( ) const;

		/**
		 * @brief Transfers the state from the stored copy to the original schedule
		 * 
		 * @details The structure of the original schedule becomes identical to the stored copy structure.
		 * The copy itself isn't destroyed.
		 */
		void apply( ) const;

		/**
		 * @brief Checks if there is a copy of some schedule stored
		 * 
		 * @return @c true if the copy of any schedule is stored, @c false otherwise
		 */
		operator bool( ) const;

	private:
		Optional<Schedule&> original_schedule;
		Optional<Schedule&> new_schedule;
		Optional<Scene&> new_scene;
	};
}