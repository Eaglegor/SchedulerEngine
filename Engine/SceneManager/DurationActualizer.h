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
			run(nullptr),
			is_dirty(true),
			actualization_in_progress(false)
		{
		}

		DurationActualizer& operator=(const DurationActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->run = rhs.run;
			actualization_in_progress = false;
			is_dirty = true;
			return *this;
		}

		DurationActualizer(DurationActualizationAlgorithm* algorithm, Run* run) :
			algorithm(algorithm),
			run(run),
			is_dirty(true),
			actualization_in_progress(false)
		{
		}

		void actualize() const
		{
			if(is_dirty && !actualization_in_progress && algorithm) {
				actualization_in_progress = true;
				algorithm->actualize(run);
				actualization_in_progress = false;
				is_dirty = false;
			}
		}

		void setDirty(bool dirty)
		{
			is_dirty = dirty;
		}
		
	private:
		Run* run;
		mutable bool is_dirty;
		mutable bool actualization_in_progress;
		DurationActualizationAlgorithm* algorithm;
	};
}
