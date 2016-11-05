#include "ChainRunValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>
#include <Engine/SceneManager/Algorithms/Validation/ProxyViolationsConsumer.h>

namespace Scheduler
{
	void ChainRunValidationAlgorithm::validate(const Run& run, ViolationsConsumer& violations_consumer) const
	{
		ProxyViolationsConsumer consumer(violations_consumer);
		for (const RunValidationAlgorithm& algorithm : algorithms)
		{
			algorithm.validate(run, consumer);
			if(consumer.getCurrentContinuancePolicy() == ValidationContinuancePolicy::INTERRUPT) break;
		}
	}

	void ChainRunValidationAlgorithm::addAlgorithm(const RunValidationAlgorithm& algorithm)
	{
		algorithms.emplace_back(algorithm);
	}
}