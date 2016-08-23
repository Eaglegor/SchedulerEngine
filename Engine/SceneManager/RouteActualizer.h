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
			stop(nullptr),
			is_dirty(true)
		{
		}

		RouteActualizer& operator=(const RouteActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->stop = rhs.stop;
			is_dirty = true;
			return *this;
		}

		RouteActualizer(RouteActualizationAlgorithm* algorithm, Stop* stop) :
			algorithm(algorithm),
			stop(stop),
			is_dirty(true)
		{
		}

		void actualize() const
		{
			if(is_dirty && algorithm) {
				algorithm->actualize(stop);
				is_dirty = false;
			}
		}

		void setDirty(bool dirty)
		{
			is_dirty = dirty;
		}
		
	private:
		Stop* stop;
		mutable bool is_dirty;
		RouteActualizationAlgorithm* algorithm;
	};
}