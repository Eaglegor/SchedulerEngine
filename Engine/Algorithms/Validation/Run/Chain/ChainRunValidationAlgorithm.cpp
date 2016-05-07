#include "ChainRunValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

bool Scheduler::ChainRunValidationAlgorithm::isValid(const Run * run) const
{
	for (RunValidationAlgorithm* algorithm : algorithms)
	{
		if (!algorithm->isValid(run)) return false;
	}
	return true;
}

void Scheduler::ChainRunValidationAlgorithm::addAlgorithm(RunValidationAlgorithm * algorithm)
{
	assert(algorithm);
	algorithms.push_back(algorithm);
}
