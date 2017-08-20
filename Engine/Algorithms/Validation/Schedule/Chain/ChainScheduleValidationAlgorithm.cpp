#include "ChainScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Algorithms/Validation/ProxyViolationsConsumer.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	void ChainScheduleValidationAlgorithm::validate(const Schedule& schedule, ViolationsConsumer& violations_consumer) const
	{
		ProxyViolationsConsumer consumer(violations_consumer);

		for(const ScheduleValidationAlgorithm& algorithm : algorithms)
		{
			algorithm.validate(schedule, consumer);
			if(consumer.getCurrentContinuancePolicy( ) == ValidationContinuancePolicy::INTERRUPT) return;
		}
	}

	void ChainScheduleValidationAlgorithm::addAlgorithm(const ScheduleValidationAlgorithm& algorithm)
	{
		algorithms.emplace_back(algorithm);
	}
}
