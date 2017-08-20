#include "ChainStopValidationAlgorithm.h"

#include <Engine/SceneManager/Algorithms/Validation/ProxyViolationsConsumer.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	void ChainStopValidationAlgorithm::validate(const Stop& stop, ViolationsConsumer& violations_consumer) const
	{
		ProxyViolationsConsumer consumer(violations_consumer);
		for(const StopValidationAlgorithm& algorithm : algorithms)
		{
			algorithm.validate(stop, consumer);
			if(consumer.getCurrentContinuancePolicy( ) == ValidationContinuancePolicy::INTERRUPT) return;
		}
	}

	void ChainStopValidationAlgorithm::addAlgorithm(const StopValidationAlgorithm& algorithm)
	{
		algorithms.push_back(algorithm);
	}
}
