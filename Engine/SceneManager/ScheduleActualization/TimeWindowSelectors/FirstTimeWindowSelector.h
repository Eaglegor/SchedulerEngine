#pragma once

#include "../TimeWindowSelector.h"

namespace Scheduler
{
	class FirstTimeWindowSelector : public TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop *stop) override;
	};
}