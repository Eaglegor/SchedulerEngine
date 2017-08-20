#pragma once

#include "../ConstraintViolation.h"
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class VehicleCapacityViolation : public ConstraintViolation
	{
	public:
		VehicleCapacityViolation(const Run& run, const Capacity& excess, Run::ConstStopIterator pos)
		    : run(run),
		      excess(excess),
		      pos(pos)
		{
		}

		VehicleCapacityViolation(const VehicleCapacityViolation& rhs)
		    : run(rhs.run),
		      excess(rhs.excess),
		      pos(rhs.pos)
		{
		}

		const Run& getRun( ) const
		{
			return run;
		}

		const Capacity& getExcess( ) const
		{
			return excess;
		}

		Run::ConstStopIterator getExcessPosition( ) const
		{
			return pos;
		}

		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::VEHICLE_CAPACITY_VIOLATION;
		}

	private:
		const Run& run;
		Capacity excess;
		Run::ConstStopIterator pos;
	};
}