#pragma once 

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Stop.h>

namespace Scheduler
{
	class DistanceAccumulator
	{
	public:
		static Distance accumulateDistance(const Stop* start_stop, const Stop* end_stop)
		{
			Distance total_distance;
			for (const Stop* stop = start_stop; stop != end_stop; stop = stop->getNextStop())
			{
				total_distance += stop->getNextRoute().getDistance();
			}
			return total_distance;
		}
	};
}