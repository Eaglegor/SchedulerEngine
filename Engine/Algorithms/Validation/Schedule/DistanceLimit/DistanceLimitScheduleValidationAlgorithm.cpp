#include "DistanceLimitScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>

bool Scheduler::DistanceLimitScheduleValidationAlgorithm::isValid(const Schedule * schedule) const
{
	if (schedule->getRuns().empty()) return true;
	if (!schedule->constraints().scheduleDistanceLimit().isSet()) return true;
	return DistanceAccumulator::accumulateDistance(schedule->getRuns().front()->getStartStop(), schedule->getRuns().back()->getEndStop()) > schedule->constraints().scheduleDistanceLimit();
}
