#include "ValidStopsRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Algorithms/Validation/ProxyViolationsConsumer.h>

namespace Scheduler
{
	void ValidStopsRunValidationAlgorithm::validate(const Run& run, ViolationsConsumer& violations_consumer) const
	{
		ProxyViolationsConsumer consumer(violations_consumer);
		
		for(const Stop& s : run.getStops())
		{
			s.validate(consumer);
			if(consumer.getCurrentContinuancePolicy() == ValidationContinuancePolicy::INTERRUPT) return;
		}
	}
}

