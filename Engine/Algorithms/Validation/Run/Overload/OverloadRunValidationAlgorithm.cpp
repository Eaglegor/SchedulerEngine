#include "OverloadRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>

bool Scheduler::OverloadRunValidationAlgorithm::isValid(const Run * run) const
{
	if (run->getVehicle() == nullptr) return true; // Have no vehicle to check with
	const auto& stops = run->getWorkStops();
	const Capacity vehicle_capacity = run->getVehicle()->getCapacity();
	bool overload = false;
	Capacity run_demand;
	for (auto stop_it = stops.begin(); stop_it != stops.end() && !overload;	++stop_it) {
		run_demand += (*stop_it)->getOperation()->getDemand();
		overload = (run_demand > vehicle_capacity);
	}
	return !overload;
}
