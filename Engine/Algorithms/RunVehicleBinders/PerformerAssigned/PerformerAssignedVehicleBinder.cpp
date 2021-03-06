#include "PerformerAssignedVehicleBinder.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	PerformerAssignedVehicleBinder::PerformerAssignedVehicleBinder( )
	{
	}

	bool PerformerAssignedVehicleBinder::bindVehicle(Run& run) const
	{
		auto iter = bindings.find(run.getSchedule( ).getPerformer( ));
		if(iter == bindings.end( )) return false;
		run.setVehicle(iter->second.get( ));
		return true;
	}

	void PerformerAssignedVehicleBinder::assign(const Performer& performer, const Vehicle& vehicle)
	{
		bindings.emplace(performer, vehicle);
	}

	void PerformerAssignedVehicleBinder::unassign(const Performer& performer, const Vehicle& vehicle)
	{
		bindings.erase(performer);
	}
}