#include "ValidStopsRunValidationAlgorithm.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>

bool Scheduler::ValidStopsRunValidationAlgorithm::isValid(const Run * run) const
{
	for (const Stop *s = run->getStartStop(); s != run->getEndStop()->getNextStop(); s = s->getNextStop())
	{
		if (!s->isValid()) return false;
	}
	return true;
}
