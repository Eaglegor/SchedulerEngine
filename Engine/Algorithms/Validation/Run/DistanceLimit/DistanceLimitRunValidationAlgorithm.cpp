#include "DistanceLimitRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>

Scheduler::DistanceLimitRunValidationAlgorithm::DistanceLimitRunValidationAlgorithm(Distance limit):
	limit(limit)
{
}

bool Scheduler::DistanceLimitRunValidationAlgorithm::isValid(const Run * run) const
{
	return DistanceAccumulator::accumulateDistance(run->getStartStop(), run->getEndStop()) > limit;
}
