#pragma once

#include "Extensions/DurationActualizationAlgorithm.h"
#include "Stop.h"

namespace Scheduler
{
	class Stop;

	class DurationActualizer
	{
	public:
		DurationActualizer() :
			algorithm(nullptr),
			stop(nullptr)
		{
		}

		DurationActualizer& operator=(const DurationActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->stop = rhs.stop;
			return *this;
		}

		DurationActualizer(DurationActualizationAlgorithm* algorithm, Stop* stop) :
			algorithm(algorithm),
			stop(stop)
		{
		}

		void actualize() const
		{
			if(algorithm) algorithm->actualize(stop);
		}

	private:
		Stop* stop;
		DurationActualizationAlgorithm* algorithm;
	};
}
