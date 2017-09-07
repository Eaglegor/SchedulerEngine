#pragma once

#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <boost/range/iterator_range.hpp>

namespace Scheduler
{
	/** 
	 * @ingroup utils
	 * 
	 * @brief Calculates the total duration of the run stops subsequence
	 * @deprecated This class is inefficient. There is much more simplier way to calculate the total stops subsequence duration
	 */
	class DurationAccumulator
	{
	public:
		static Duration accumulateDuration(const Stop& start_stop, const Stop& end_stop)
		{
			Duration total_duration;
			for(const Stop& stop : boost::iterator_range<Stop::const_iterator_type>(start_stop.getRun( ).findStop(start_stop), end_stop.getRun( ).findStop(end_stop)))
			{
				total_duration += stop.getDuration( );
				if(&stop != &end_stop)
				{
					const Stop& next_stop = *std::next(stop.getRun( ).findStop(stop));
					total_duration += next_stop.getAllocationTime( ).getStartTime( ) - stop.getAllocationTime( ).getEndTime( );
				}
			}
			return total_duration;
		}
	};
}