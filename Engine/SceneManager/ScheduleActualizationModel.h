#pragma once

#include <Engine/Utils/ReferenceWrapper.h>


namespace Scheduler
{
	class RouteActualizationAlgorithm;
	class ArrivalTimeActualizationAlgorithm;
	class DurationActualizationAlgorithm;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Algorithms used to perform schedule actualization after the structural changes
	 * 
	 * @details There are multiple problems that need to be solved during the scheduling process
	 * One of these problems is to update the schedule's data sensitive to the schedule's structural state
	 * These data are:
	 * - Inter-order route lengths in the sequence
	 * - Planned orders service durations
	 * - Arrival/Departure times for scheduled orders services
	 * 
	 * These data have to be updated every time when:
	 * - New orders become planned to the schedule
	 * - Some orders become unplanned from the schedule
	 * - The sequence of orders in the schedule changes
	 * 
	 * To simplify algorithms development these recalculations are managed by the scene manager.
	 * The user just has to provide the algorithms that perform the actual job.
	 * 
	 * Schedule actualization model contains a set of these algorithms and is used by scene
	 * manager to actualize the mentioned state-sensitive data.
	 */
	class ScheduleActualizationModel
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @details Initializes all algorithms with empty ones (no recalculations are performed)
		 */
		ScheduleActualizationModel( );

		/**
		 * @brief Returns current route actualization algorithm
		 * 
		 * @return Current route actualization algorithm
		 */
		const RouteActualizationAlgorithm& getRouteActualizationAlgorithm( ) const;

		/**
		 * @brief Returns current arrival time actualization algorithm
		 * 
		 * @return Current arrival time actualization algorithm
		 */
		const ArrivalTimeActualizationAlgorithm& getArrivalTimeActualizationAlgorithm( ) const;

		/**
		 * @brief Returns current duration actualization algorithm
		 * 
		 * @return Current duration actualization algorithm
		 */
		const DurationActualizationAlgorithm& getDurationActualizationAlgorithm( ) const;

		/**
		 * @brief Sets current route actualization algorithm
		 * 
		 * @param algorithm New route actualization algorithm
		 */
		void setRouteActualizationAlgorithm(const RouteActualizationAlgorithm& algorithm);

		/**
		 * @brief Sets current arrival time actualization algorithm
		 * 
		 * @param algorithm New arrival time actualization algorithm
		 */
		void setArrivalTimeActualizationAlgorithm(const ArrivalTimeActualizationAlgorithm& algorithm);
		
		/**
		 * @brief Sets current duration actualization algorithm
		 * 
		 * @param algorithm New duration actualization algorithm
		 */
		void setDurationActualizationAlgorithm(const DurationActualizationAlgorithm& algorithm);

	private:
		ReferenceWrapper<const RouteActualizationAlgorithm> route_actualization_algorithm;
		ReferenceWrapper<const ArrivalTimeActualizationAlgorithm> arrival_time_actualization_algorithm;
		ReferenceWrapper<const DurationActualizationAlgorithm> duration_actualization_algorithm;
	};
}