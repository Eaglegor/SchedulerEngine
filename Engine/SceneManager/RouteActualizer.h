#pragma once

namespace Scheduler
{
	class Stop;
	class RouteActualizationAlgorithm;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Updates next route of the stop
	 * 
	 * @details Internally this class uses dirty flag to reduce recalculations. This flag must be set
	 * explicitly to notify actualizer that the object has changed and recalculation must be 
	 * performed upon next call to actualize()
	 */
	class RouteActualizer
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param stop Stop to call actualizer for
		 */
		explicit RouteActualizer(Stop& stop);

		/**
		 * @brief Recalculates next route for the stop
		 * 
		 * @details If dirty flag is set to false this method just returns. 
		 * If dirty flag is set to true recalculation is performed:
		 *   -# RouteActualizationAlgorithm is retrieved from the ScheduleActualizationModel
		 *   -# Algorithm is called to update the next route of the stop
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
		inline const RouteActualizationAlgorithm& getAlgorithm( ) const;

		Stop& stop;
		mutable bool is_dirty;
	};
}