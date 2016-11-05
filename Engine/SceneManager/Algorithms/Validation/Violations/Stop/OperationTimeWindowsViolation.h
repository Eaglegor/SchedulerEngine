#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Stop;
	
	class OperationTimeWindowsViolation : public ConstraintViolation
	{
	public:
		OperationTimeWindowsViolation(const Stop& stop, const Duration& excess):
		stop(stop),
		excess(excess)
		{}
		
		OperationTimeWindowsViolation(const OperationTimeWindowsViolation& rhs):
		stop(rhs.stop),
		excess(rhs.excess)
		{}
		
		const Duration& getExcess() const
		{
			return excess;
		}
		
		const Stop& getStop() const
		{
			return stop;
		}
		
	private:
		const Stop& stop;
		Duration excess;
	};
}