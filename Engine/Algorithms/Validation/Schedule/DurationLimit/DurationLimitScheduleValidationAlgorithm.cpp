#include "DurationLimitScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DurationAccumulator.h>

Scheduler::DurationLimitScheduleValidationAlgorithm::DurationLimitScheduleValidationAlgorithm(Duration limit):
	limit(limit)
{
}

bool Scheduler::DurationLimitScheduleValidationAlgorithm::isValid(const Schedule * schedule) const
{
	if (schedule->getRuns().empty()) return true;
	return DurationAccumulator::accumulateDuration(schedule->getRuns().front()->getStartStop(), schedule->getRuns().back()->getEndStop()) > limit;
}
