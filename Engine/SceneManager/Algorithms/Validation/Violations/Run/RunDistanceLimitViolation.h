#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Distance.h>

namespace Scheduler
{
	class Run;

	class RunDistanceLimitViolation : public ConstraintViolation
	{
	public:
		RunDistanceLimitViolation(const Run& run, const Distance excess)
		    : run(run),
		      excess(excess)
		{
		}

		RunDistanceLimitViolation(const RunDistanceLimitViolation& rhs)
		    : run(rhs.run),
		      excess(rhs.excess)
		{
		}

		const Run& getRun( ) const
		{
			return run;
		}

		const Distance& getExcess( ) const
		{
			return excess;
		}

		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::RUN_DISTANCE_LIMIT_VIOLATION;
		}

	private:
		const Run& run;
		Distance excess;
	};
}