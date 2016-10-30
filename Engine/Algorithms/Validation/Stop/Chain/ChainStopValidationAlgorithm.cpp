#include "ChainStopValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

bool Scheduler::ChainStopValidationAlgorithm::isValid(const Stop& stop) const
{
	for (const StopValidationAlgorithm& algorithm : algorithms)
	{
		if (!algorithm.isValid(stop)) return false;
	}
	return true;
}

void Scheduler::ChainStopValidationAlgorithm::addAlgorithm(const StopValidationAlgorithm& algorithm)
{
	algorithms.push_back(algorithm);
}
