#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Run;
	
	class RunWorkingTimeLimitViolation : public ConstraintViolation
	{
	public:
		RunWorkingTimeLimitViolation(const Run& run, const Duration excess):
		run(run),
		excess(excess)
		{
		}
		
		RunWorkingTimeLimitViolation(const RunWorkingTimeLimitViolation& rhs):
		run(rhs.run),
		excess(rhs.excess)
		{
		}
		
		const Run& getSchedule() const
		{
			return run;
		}
		
		const Duration& getExcess() const
		{
			return excess;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::RUN_WORKING_TIME_LIMIT_VIOLATION;
		}
		
	private:
		const Run& run;
		Duration excess;
	};
}