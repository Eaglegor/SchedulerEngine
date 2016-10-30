#include "RouteActualizer.h"

#include "Algorithms/Actualization/RouteActualizationAlgorithm.h"
#include "Stop.h"
#include "Run.h"
#include "Schedule.h"

namespace Scheduler
{
	RouteActualizer::RouteActualizer(Stop& stop):
	stop(stop),
	is_dirty(true)
	{
	}

	void RouteActualizer::actualize() const
	{
		if(is_dirty) {
			getAlgorithm().actualize(stop);
			is_dirty = false;
		}
	}
	
	void RouteActualizer::setDirty(bool dirty)
	{
		is_dirty = dirty;
	}
	
	const RouteActualizationAlgorithm& RouteActualizer::getAlgorithm() const
	{
		return stop.getRun().getSchedule().getActualizationModel().getRouteActualizationAlgorithm();
	}

}