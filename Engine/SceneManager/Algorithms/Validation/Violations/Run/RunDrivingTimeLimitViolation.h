#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Run;
	
	class RunDrivingTimeLimitViolation : public ConstraintViolation
	{
	public:
		RunDrivingTimeLimitViolation(const Run& run, const Duration excess):
		run(run),
		excess(excess)
		{
		}
		
		RunDrivingTimeLimitViolation(const RunDrivingTimeLimitViolation& rhs):
		run(rhs.run),
		excess(rhs.excess)
		{
		}
		
		const Run& getRun() const
		{
			return run;
		}
		
		const Duration& getExcess() const
		{
			return excess;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::RUN_DRIVING_TIME_LIMIT_VIOLATION;
		}
		
	private:
		const Run& run;
		Duration excess;
	};
}