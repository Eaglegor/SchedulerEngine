#include "DurationLimitScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DurationAccumulator.h>

bool Scheduler::DurationLimitScheduleValidationAlgorithm::isValid(const Schedule * schedule) const
{
	if (schedule->getRuns().empty()) return true;
	if (!schedule->constraints().scheduleWorkingTimeLimit().isSet()) return true;
	return DurationAccumulator::accumulateDuration(schedule->getRuns().front()->getStartStop(), schedule->getRuns().back()->getEndStop()) > schedule->constraints().scheduleWorkingTimeLimit();
}
