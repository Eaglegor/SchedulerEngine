#pragma once

#include "Extensions/ArrivalTimeActualizationAlgorithm.h"

namespace Scheduler
{
	class Schedule;

	class ArrivalTimeActualizer
	{
	public:
		ArrivalTimeActualizer() :
			algorithm(nullptr),
			schedule(nullptr)
		{
		}

		ArrivalTimeActualizer& operator=(const ArrivalTimeActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->schedule = rhs.schedule;
			return *this;
		}

		ArrivalTimeActualizer(ArrivalTimeActualizationAlgorithm* algorithm, Schedule* schedule) :
			algorithm(algorithm),
			schedule(schedule)
		{
		}

		void actualize() const
		{
			if(algorithm) algorithm->actualize(schedule);
		}

	private:
		Schedule* schedule;
		ArrivalTimeActualizationAlgorithm* algorithm;
	};
}
