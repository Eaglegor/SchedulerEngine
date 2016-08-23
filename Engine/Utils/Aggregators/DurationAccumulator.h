#pragma once 

#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Stop.h>

namespace Scheduler
{
	class DurationAccumulator
	{
	public:
		static Duration accumulateDuration(const Stop* start_stop, const Stop* end_stop)
		{
			Duration total_duration;
			for (const Stop* stop = start_stop; stop != end_stop; stop = stop->next())
			{
				total_duration += stop->getDuration();
				if (stop->next())
				{
					total_duration += stop->next()->getAllocationTime().getStartTime() - stop->getAllocationTime().getEndTime();
				}
			}
			return total_duration;
		}
	};
}