#pragma once
#include <vector>
#include <memory>

#include <Engine/Concepts/Capacity.h>
#include "TimeWindowsConstraint.h"

namespace Scheduler
{
	using Demand = Capacity;

	class OperationConstraints
	{
	public:
		const TimeWindowsConstraint* timeWindows() const;
		const Demand* demand() const;

		void addConstraint(const TimeWindowsConstraint& time_windows);
		void addConstraint(const Demand& demand);
		
		void removeTimeWindowsConstraint();
		void removeDemandConstraint();

	private:
		std::unique_ptr<TimeWindowsConstraint> _time_windows;
		std::unique_ptr<Demand> _demand;
	};
}