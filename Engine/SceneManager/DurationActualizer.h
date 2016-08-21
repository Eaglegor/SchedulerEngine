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
			stop(nullptr),
			is_dirty(true)
		{
		}

		DurationActualizer& operator=(const DurationActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->stop = rhs.stop;
			is_dirty = true;
			return *this;
		}

		DurationActualizer(DurationActualizationAlgorithm* algorithm, Stop* stop) :
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
		DurationActualizationAlgorithm* algorithm;
	};
}
