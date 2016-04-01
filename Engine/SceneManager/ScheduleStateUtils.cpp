#include "ScheduleStateUtils.h"
#include "Schedule.h"
#include "Run.h"
#include "WorkStop.h"

namespace Scheduler
{
	void ScheduleStateUtils::copyState(const Schedule * from, Schedule * to)
	{
		to->clear();

		for (Run* r : from->getRuns())
		{
			Run* new_run = to->createRun(r->getStartLocation(), r->getEndLocation());
			new_run->setVehicle(r->getVehicle());
			
			for (const Operation* operation : r->getStartStop()->getOperations())
			{
				new_run->allocateStartOperation(operation);
			}

			for (const WorkStop* stop : r->getWorkStops())
			{
				new_run->allocateWorkOperation(stop->getOperation());
			}

			for (const Operation* operation : r->getEndStop()->getOperations())
			{
				new_run->allocateEndOperation(operation);
			}
		}
	}
}