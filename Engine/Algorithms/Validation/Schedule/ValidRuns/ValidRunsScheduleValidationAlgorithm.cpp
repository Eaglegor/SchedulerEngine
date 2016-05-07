#include "ValidRunsScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

bool Scheduler::ValidRunsScheduleValidationAlgorithm::isValid(const Schedule * schedule) const
{
	for (const Run *r : schedule->getRuns())
	{
		if (!r->isValid()) return false;
	}
	return true;
}
