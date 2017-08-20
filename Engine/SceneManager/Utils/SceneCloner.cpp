#include "SceneCloner.h"
#include "../Schedule.h"
#include "../Run.h"
#include "../WorkStop.h"
#include "../Vehicle.h"
#include "../Scene.h"
#include "../WorkOperation.h"
#include "../SceneContext.h"
#include <unordered_map>
#include <type_traits>

namespace Scheduler 
{
		void SceneCloner::cloneScheduleProperties(const Schedule& from, Schedule& to)
		{
			to.setActualizationModel(from.getActualizationModel());
			to.setValidationModel(from.getValidationModel());
			to.setName(from.getName());
			to.setShift(from.getShift());
			to.constraints() = from.constraints();
			to.setRunVehicleBinder(from.getRunVehicleBinder());
		}
	
		void SceneCloner::cloneScheduleAssignments(const Schedule& from, Schedule& to)
		{
			to.clear();
			
			for (Run& r : from.getRuns())
			{
				Run& new_run = *(to.createRun(to.getRuns().end(), r.getStartStop().getLocation(), r.getEndStop().getLocation()));
				new_run.setVehicle(r.getVehicle());
				
				for (const WorkStop& stop : r.getWorkStops())
				{
					WorkStop& new_stop = *new_run.allocateOrder(new_run.getWorkStops().end(), stop.getOperation().getOrder());
					new_stop.setDuration(stop.getDuration());
					new_stop.setAllocationTime(stop.getAllocationTime());
				}

				new_run.getStartStop().setDuration(r.getStartStop().getDuration());
				new_run.getStartStop().setAllocationTime(r.getStartStop().getAllocationTime());
				
				new_run.getEndStop().setDuration(r.getEndStop().getDuration());
				new_run.getEndStop().setAllocationTime(r.getEndStop().getAllocationTime());
			}
		}
	
		void SceneCloner::cloneScheduleState(const Schedule& from, Schedule& to) 
		{
			cloneScheduleProperties(from, to);
			cloneScheduleAssignments(from, to);			
		}
	
		void SceneCloner::cloneSceneSchedulesPool(const Scene& from, Scene& to)
		{
			assert(from.getContext() == to.getContext());
			assert(to.getSchedules().empty());
			
			for(const Schedule& from_schedule :from.getSchedules())
			{
				Schedule& to_schedule = to.createSchedule(from_schedule.getPerformer());
				cloneScheduleProperties(from_schedule, to_schedule);
			}
		}

		void SceneCloner::cloneSceneAssignments(const Scene& from, Scene& to)
		{
			assert(from.getContext() == to.getContext());
			assert(from.getSchedules().size() == to.getSchedules().size());
			
			auto from_iter = from.getSchedules().begin();
			auto to_iter = to.getSchedules().begin();
			
			for( ; from_iter != from.getSchedules().end() && to_iter != to.getSchedules().end(); ++from_iter, ++to_iter)
			{
				cloneScheduleAssignments(*from_iter, *to_iter);
			}
		}
}
