#pragma once

#include <Engine/Concepts/TimeWindow.h>

namespace Scheduler
{
	class Stop;
	
	class FirstTimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(const Stop &stop) const;
	};
}