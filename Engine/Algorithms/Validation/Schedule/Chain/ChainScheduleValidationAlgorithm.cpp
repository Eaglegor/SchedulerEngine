#include "ChainScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

bool Scheduler::ChainScheduleValidationAlgorithm::isValid(const Schedule * schedule) const
{
	for (ScheduleValidationAlgorithm* algorithm : algorithms)
	{
		if (!algorithm->isValid(schedule)) return false;
	}
	return true;
}

void Scheduler::ChainScheduleValidationAlgorithm::addAlgorithm(ScheduleValidationAlgorithm * algorithm)
{
	assert(algorithm);
	algorithms.push_back(algorithm);
}
