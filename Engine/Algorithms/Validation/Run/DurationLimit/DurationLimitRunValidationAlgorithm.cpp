#include "DurationLimitRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DurationAccumulator.h>

bool Scheduler::DurationLimitRunValidationAlgorithm::isValid(const Run * run) const
{
	if (!run->getSchedule()->constraints().runWorkingTimeLimit().isSet()) return true;
	return DurationAccumulator::accumulateDuration(run->getStartStop(), run->getEndStop()) > run->getSchedule()->constraints().runWorkingTimeLimit();
}
