#include "OverloadRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/DepotOperation.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>

namespace Scheduler
{
	void OverloadRunValidationAlgorithm::validate(const Run& run, ViolationsConsumer& violations_consumer) const
	{
		if(!violations_consumer.supportsViolationType(ConstraintViolationType::VEHICLE_CAPACITY_VIOLATION)) return;
		
		if(!run.getVehicle()) return;
		if(!run.getVehicle()->constraints().capacity().isSet()) return;
		
		const Capacity &vehicle_capacity = run.getVehicle()->constraints().capacity().get();
		
		Capacity current_demand;
		
		
		
		for(const Operation& operation : run.getStartStop().getOperations())
		{
			if(operation.constraints().demand().isSet()) current_demand += operation.constraints().demand().get();
		}
		
		if(current_demand > vehicle_capacity)
		{
			auto continuation_policy = violations_consumer.consumeViolation(VehicleCapacityViolation(run, current_demand - vehicle_capacity, run.getStops().begin()));
			if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
		}


		
		const auto& work_stops = run.getWorkStops();
		
		for(const WorkStop& stop : run.getWorkStops())
		{
			if(!stop.getOperation().constraints().demand().isSet()) continue;
			current_demand += stop.getOperation().constraints().demand();
			if(current_demand > vehicle_capacity)
			{
				auto continuation_policy = violations_consumer.consumeViolation(VehicleCapacityViolation(run, current_demand - vehicle_capacity, run.findStop(stop) ));
				if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
			}
		}

		
		
		for(const Operation& operation : run.getEndStop().getOperations())
		{
			if(operation.constraints().demand().isSet()) current_demand += operation.constraints().demand().get();
		}
		
		if(current_demand > vehicle_capacity)
		{
			auto continuation_policy = violations_consumer.consumeViolation(VehicleCapacityViolation(run, current_demand - vehicle_capacity, std::prev(run.getStops().end())));
			if(continuation_policy == ValidationContinuancePolicy::INTERRUPT) return;
		}
	}
}
