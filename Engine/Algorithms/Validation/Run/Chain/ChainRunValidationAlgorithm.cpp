#include "ChainRunValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	bool ChainRunValidationAlgorithm::isValid(const Run &run) const
	{
		for (const RunValidationAlgorithm& algorithm : algorithms)
		{
			if (!algorithm.isValid(run)) return false;
		}
		return true;
	}

	void ChainRunValidationAlgorithm::addAlgorithm(const RunValidationAlgorithm& algorithm)
	{
		algorithms.emplace_back(algorithm);
	}
}