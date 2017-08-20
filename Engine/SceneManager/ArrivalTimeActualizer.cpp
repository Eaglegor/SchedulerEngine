#include "ArrivalTimeActualizer.h"

#include "Algorithms/Actualization/ArrivalTimeActualizationAlgorithm.h"
#include "Schedule.h"

namespace Scheduler
{
	ArrivalTimeActualizer::ArrivalTimeActualizer(Schedule& schedule)
	    : schedule(schedule),
	      is_dirty(true),
	      actualization_in_progress(false)
	{
	}

	void ArrivalTimeActualizer::actualize( ) const
	{
		if(is_dirty && !actualization_in_progress)
		{
			actualization_in_progress = true;
			getAlgorithm( ).actualize(schedule);
			is_dirty                  = false;
			actualization_in_progress = false;
		}
	}

	void ArrivalTimeActualizer::setDirty(bool dirty)
	{
		is_dirty = dirty;
	}

	const ArrivalTimeActualizationAlgorithm& ArrivalTimeActualizer::getAlgorithm( ) const
	{
		return schedule.getActualizationModel( ).getArrivalTimeActualizationAlgorithm( );
	}
}