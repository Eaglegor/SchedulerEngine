#include "DistanceLimitRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>

namespace Scheduler
{
	void DistanceLimitRunValidationAlgorithm::validate(const Run& run, ViolationsConsumer& violations_consumer) const
	{
		if (!run.getSchedule().constraints().runDistanceLimit().isSet()) return;
		
		Distance run_distance = std::accumulate(run.getStops().begin(), run.getStops().end(), Distance(0), DistanceAccumulator());
		const Distance& run_distance_limit = run.getSchedule().constraints().runDistanceLimit();
		
		if( run_distance > run_distance_limit )
		{
			violations_consumer.consumeViolation(RunDistanceLimitViolation(run, run_distance - run_distance_limit));
		}
	}
}
