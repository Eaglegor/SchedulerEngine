#include "ChainStopValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

bool Scheduler::ChainStopValidationAlgorithm::isValid(const Stop * stop) const
{
	for (StopValidationAlgorithm* algorithm : algorithms)
	{
		if (!algorithm->isValid(stop)) return false;
	}
	return true;
}

void Scheduler::ChainStopValidationAlgorithm::addAlgorithm(StopValidationAlgorithm * algorithm)
{
	assert(algorithm);
	algorithms.push_back(algorithm);
}
