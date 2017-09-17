#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Stop;
	
	/**
	 * @brief Algorithm used to update next route for the stop
	 * 
	 * @sa @ref schedule_actualization
	 */
	class RouteActualizationAlgorithm : public Algorithm
	{
	public:
		/**
		 * @brief Updates next route for the stop
		 * 
		 * @param stop Stop to update next route for
		 */
		virtual void actualize(Stop& stop) const = 0;
	};
}