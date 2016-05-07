#include "DurationLimitRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DurationAccumulator.h>

Scheduler::DurationLimitRunValidationAlgorithm::DurationLimitRunValidationAlgorithm(const Duration &limit):
	limit(limit)
{
}

bool Scheduler::DurationLimitRunValidationAlgorithm::isValid(const Run * run) const
{
	return DurationAccumulator::accumulateDuration(run->getStartStop(), run->getEndStop()) > limit;
}
