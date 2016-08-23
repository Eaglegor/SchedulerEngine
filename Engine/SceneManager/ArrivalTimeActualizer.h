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
			schedule(nullptr),
			is_dirty(true),
			actualization_in_progress(false)
		{
		}

		ArrivalTimeActualizer& operator=(const ArrivalTimeActualizer& rhs)
		{
			this->algorithm = rhs.algorithm;
			this->schedule = rhs.schedule;
			this->is_dirty = true;
			this->actualization_in_progress = false;
			return *this;
		}

		ArrivalTimeActualizer(ArrivalTimeActualizationAlgorithm* algorithm, Schedule* schedule) :
			algorithm(algorithm),
			schedule(schedule),
			is_dirty(true),
			actualization_in_progress(false)
		{
		}

		void actualize() const
		{
			if(is_dirty && !actualization_in_progress && algorithm) {
				actualization_in_progress = true;
				algorithm->actualize(schedule);
				is_dirty = false;
				actualization_in_progress = false;
			}
		}

		void setDirty(bool dirty)
		{
			is_dirty = dirty;
		}
		
	private:
		mutable bool is_dirty;
		mutable bool actualization_in_progress;
		Schedule* schedule;
		ArrivalTimeActualizationAlgorithm* algorithm;
	};
}
