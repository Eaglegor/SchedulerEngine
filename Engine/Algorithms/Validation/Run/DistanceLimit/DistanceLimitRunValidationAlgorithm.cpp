#include "DistanceLimitRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>

bool Scheduler::DistanceLimitRunValidationAlgorithm::isValid(const Run * run) const
{
	if (!run->getSchedule()->constraints().runDistanceLimit().isSet()) return true;
	return DistanceAccumulator::accumulateDistance(run->getStartStop(), run->getEndStop()) > run->getSchedule()->constraints().runDistanceLimit();
}
