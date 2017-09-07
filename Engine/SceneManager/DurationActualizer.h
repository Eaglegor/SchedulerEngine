#pragma once

namespace Scheduler
{
	class Run;
	class DurationActualizationAlgorithm;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Updates durations of all stops in the run
	 * 
	 * @details Internally this class uses dirty flag to reduce recalculations. This flag must be set
	 * explicitly to notify actualizer that the object has changed and recalculation must be 
	 * performed upon next call to actualize()
	 */
	class DurationActualizer
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run to call actualizer for
		 */
		explicit DurationActualizer(Run& run);

		/**
		 * @brief Recalculates durations for all stops in the run
		 * 
		 * @details If dirty flag is set to false this method just returns. 
		 * If dirty flag is set to true recalculation is performed:
		 *   -# DurationActualizationAlgorithm is retrieved from the ScheduleActualizationModel
		 *   -# Algorithm is called to update durations in the run
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
		inline const DurationActualizationAlgorithm& getAlgorithm( ) const;

		Run& run;
		mutable bool is_dirty;
		mutable bool actualization_in_progress;
	};
}
