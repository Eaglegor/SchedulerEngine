#pragma once

#include "../TimeWindowSelector.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	/// Selects the first of the multiple stop time windows or creates a broad time window if there are no time windows specified for any stop's operation
	class SCHEDULERENGINE_EXPORT FirstTimeWindowSelector : public TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop *stop) override;
	};
}