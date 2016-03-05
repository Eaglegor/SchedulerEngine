#pragma once

#include "../TimeWindowSelector.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT FirstTimeWindowSelector : public TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop *stop) override;
	};
}