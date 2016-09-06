#include "SceneCloner.h"
#include "../Schedule.h"
#include "../Run.h"
#include "../WorkStop.h"
#include "../Vehicle.h"
#include "../Scene.h"
#include <unordered_map>

namespace Scheduler {
		void SceneCloner::cloneScheduleState(const Schedule* from, Schedule* to) 
		{
			to->setActualizationModel(from->getActualizationModel());
			to->setValidationModel(from->getValidationModel());
			to->setName(from->getName());
			to->setShift(from->getShift());
			to->constraints() = from->constraints();
			
			to->clear();
			
			for (Run* r : from->getRuns())
			{
				Run* new_run = *(to->createRun(to->getRuns().end(), r->getStartStop()->getLocation(), r->getEndStop()->getLocation()));
				new_run->setVehicle(r->getVehicle());
				
				for (const Operation* operation : r->getStartStop()->getOperations())
				{
					new_run->allocateStartOperation(operation);
				}
				new_run->getStartStop()->setDuration(r->getStartStop()->getDuration());
				new_run->getStartStop()->setAllocationTime(r->getStartStop()->getAllocationTime());

				for (const WorkStop* stop : r->getWorkStops())
				{
					WorkStop* new_stop = *new_run->createWorkStop(new_run->getWorkStops().end(), stop->getOperation());
					new_stop->setDuration(stop->getDuration());
					new_stop->setAllocationTime(stop->getAllocationTime());
				}

				for (const Operation* operation : r->getEndStop()->getOperations())
				{
					new_run->allocateEndOperation(operation);
				}
				new_run->getEndStop()->setDuration(r->getEndStop()->getDuration());
				new_run->getEndStop()->setAllocationTime(r->getEndStop()->getAllocationTime());
			}
		}
}