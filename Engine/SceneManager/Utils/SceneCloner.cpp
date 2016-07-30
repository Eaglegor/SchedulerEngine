#include "SceneCloner.h"
#include "../Schedule.h"
#include "../Run.h"
#include "../WorkStop.h"
#include "../Vehicle.h"
#include "../Scene.h"
#include <unordered_map>

namespace Scheduler {
	
		void SceneCloner::cloneSceneState(Scene* from, Scene* to)
		{
			for(Schedule* schedule : from->getSchedules())
			{
				Schedule* new_schedule = to->createSchedule(schedule->getPerformer());
				cloneScheduleState(schedule, new_schedule);
			}
		}
		
		void SceneCloner::cloneScheduleState(const Schedule* from, Schedule* to) 
		{
			to->setActualizationModel(from->getActualizationModel());
			to->setValidationModel(from->getValidationModel());
			to->setName(from->getName());
			to->setDepotLocation(from->getDepotLocation());
			to->setShift(from->getShift());
			to->constraints() = from->constraints();
			
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