#pragma once

#include "Extensions/RouteActualizationAlgorithm.h"

namespace Scheduler
{
	class Stop;

	class RouteActualizer
	{
	public:
		RouteActualizer() :
			algorithm(nullptr),
			stop(nullptr)
		{
		}

		RouteActualizer& operator=(const RouteActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->stop = rhs.stop;
			return *this;
		}

		RouteActualizer(RouteActualizationAlgorithm* algorithm, Stop* stop) :
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
		RouteActualizationAlgorithm* algorithm;
	};
}