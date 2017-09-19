#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Run;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Algorithm used to update the durations of the stops in the run
	 * 
	 * @sa @ref schedule_actualization
	 */
	class DurationActualizationAlgorithm : public Algorithm
	{
	public:
		/**
		 * @brief Updates durations of the stops in the run
		 * 
		 * @param run Run to actualize durations in
		 */
		virtual void actualize(Run& run) const = 0;
	};
}