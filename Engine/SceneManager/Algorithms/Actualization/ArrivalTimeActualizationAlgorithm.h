#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Schedule;

	/**
	 * @brief Algorithm used to update the arrival times of the stops in the schedule
	 * 
	 * @sa @ref schedule_actualization
	 */
	class ArrivalTimeActualizationAlgorithm : public Algorithm
	{
	public:
		/**
		 * @brief Updates arrival times of the stops in the schedule
		 * 
		 * @param schedule Schedule to actualize arrival times in
		 */
		virtual void actualize(Schedule& schedule) const = 0;
	};
}