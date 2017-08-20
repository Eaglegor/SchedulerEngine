#include "ValidRunsScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Algorithms/Validation/ProxyViolationsConsumer.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	void ValidRunsScheduleValidationAlgorithm::validate(const Schedule& schedule, ViolationsConsumer& violations_consumer) const
	{
		ProxyViolationsConsumer consumer(violations_consumer);

		for(const Run& r : schedule.getRuns( ))
		{
			r.validate(consumer);
			if(consumer.getCurrentContinuancePolicy( ) == ValidationContinuancePolicy::INTERRUPT) return;
		}
	}
}
