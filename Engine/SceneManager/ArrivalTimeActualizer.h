#pragma once

namespace Scheduler
{
	class Schedule;
	class ArrivalTimeActualizationAlgorithm;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Recalculates arrival times of all stops in the schedule
	 * 
	 * @details Internally this class uses dirty flag to reduce recalculations. This flag must be set
	 * explicitly to notify actualizer that the object has changed and recalculation must be 
	 * performed upon next call to actualize()
	 */
	class ArrivalTimeActualizer
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param schedule Schedule to call the actualizer for
		 */
		explicit ArrivalTimeActualizer(Schedule& schedule);

		/**
		 * @brief Recalculates arrival times for all stops in the schedule
		 * 
		 * @details If dirty flag is set to false this method just returns. 
		 * If dirty flag is set to true recalculation is performed:
		 *   -# ArrivalTimeActualizationAlgorithm is retrieved from the ScheduleActualizationModel
		 *   -# Algorithm is called to update arrival times in the schedule
		 *   -# Dirty flag is set to false
		 */
		void actualize( ) const;

		/**
		 * @brief Sets dirty flag to the specified value
		 * 
		 * @param dirty New value of dirty flag
		 */
		void setDirty(bool dirty);

	private:
		inline const ArrivalTimeActualizationAlgorithm& getAlgorithm( ) const;

		mutable bool is_dirty;
		mutable bool actualization_in_progress;
		Schedule& schedule;
	};
}
