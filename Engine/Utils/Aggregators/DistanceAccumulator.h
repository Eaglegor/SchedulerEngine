#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <boost/range/iterator_range.hpp>

namespace Scheduler
{
	/** 
	 * @brief Sums the passed distance and the passed stop's next route distance
	 */
	class DistanceAccumulator
	{
	public:
		/** 
		 * @brief returns the sum of the passed distance and the passed stop's next route distance
		 * 
		 * @param distance The base distance
		 * @param stop The stop to get the next route distance from
		 * 
		 * @return The sum of the passed distance and the stop's next route distance
		 */
		Distance operator( )(const Distance& distance, const Stop& stop) const
		{
			return distance + stop.getNextRoute( ).getDistance( );
		}
	};

	/** 
	 * @brief Calculates the total distance of the run stops subsequence
	 */
	class DistanceAccumulatorAlg
	{
	public:
		/** 
		 * @brief Sums the next route distances of all stops starting from start_stop (inclusive) and ending at end_stop (exclusive)
		 * 
		 * @param start_stop The start of the distance calculation range
		 * @param end_stop The end of the distance calculation range
		 * 
		 * @return The total routes distance in the specified range
		 */
		static Distance accumulateDistance(const Run::ConstStopIterator& start_stop, const Run::ConstStopIterator& end_stop)
		{
			Distance total_distance;

			for(const Stop& stop : boost::iterator_range<Stop::const_iterator_type>(start_stop, end_stop))
			{
				total_distance += stop.getNextRoute( ).getDistance( );
			}
			return total_distance;
		}
	};
}