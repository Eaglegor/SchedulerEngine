#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Schedule;

	class ArrivalTimeActualizationAlgorithm : public Algorithm
	{
	public:
		virtual void actualize(Schedule& schedule) const = 0;
	};
}