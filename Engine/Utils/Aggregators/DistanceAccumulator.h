#pragma once 

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Run.h>
#include <boost/range/iterator_range.hpp>

namespace Scheduler
{
	class DistanceAccumulator
	{
	public:
		Distance operator()(const Distance& distance, const Stop& stop) const
		{
			return distance + stop.getNextRoute().getDistance();
		}
	};
	
	class DistanceAccumulatorAlg
	{
	public:
		static Distance accumulateDistance(const Run::ConstStopIterator& start_stop, const Run::ConstStopIterator& end_stop)
		{
			Distance total_distance;
			
			for(const Stop& stop : boost::iterator_range<Stop::const_iterator_type>(start_stop, end_stop))
			{
				total_distance += stop.getNextRoute().getDistance();
			}
			return total_distance;
		}
	};
}