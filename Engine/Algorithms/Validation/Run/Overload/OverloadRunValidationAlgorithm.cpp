#include "OverloadRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>

bool Scheduler::OverloadRunValidationAlgorithm::isValid(const Run * run) const
{
	if (run->getVehicle() == nullptr) return true; // Have no vehicle to check with
	if (!run->getVehicle()->constraints().capacity().isSet()) return true; // No capacity limit
	const auto& stops = run->getWorkStops();
	const Capacity &vehicle_capacity = run->getVehicle()->constraints().capacity().get();
	bool overload = false;
	Capacity run_demand;
	for (auto stop_it = stops.begin(); stop_it != stops.end() && !overload;	++stop_it) {
		if (!(*stop_it)->getOperation()->constraints().demand().isSet()) continue;
		const Capacity& demand = (*stop_it)->getOperation()->constraints().demand().get();
		run_demand += demand;
		overload = (run_demand > vehicle_capacity);
	}
	return !overload;
}
