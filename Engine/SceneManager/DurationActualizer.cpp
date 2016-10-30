#include "DurationActualizer.h"
#include "ScheduleActualizationModel.h"
#include "Run.h"
#include "Schedule.h"
#include "Algorithms/Actualization/DurationActualizationAlgorithm.h"

namespace Scheduler
{
	
	DurationActualizer::DurationActualizer(Run& run):
	run(run),
	is_dirty(true),
	actualization_in_progress(false)
	{
	}

	void DurationActualizer::actualize() const
	{
		if(is_dirty && !actualization_in_progress) {
			actualization_in_progress = true;
			getAlgorithm().actualize(run);
			actualization_in_progress = false;
			is_dirty = false;
		}
	}
	
	void DurationActualizer::setDirty(bool dirty)
	{
		is_dirty = dirty;
	}
	
	const DurationActualizationAlgorithm& DurationActualizer::getAlgorithm() const
	{
		return run.getSchedule().getActualizationModel().getDurationActualizationAlgorithm();
	}

}