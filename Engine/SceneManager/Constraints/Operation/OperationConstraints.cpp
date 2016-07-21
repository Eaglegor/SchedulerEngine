#include "OperationConstraints.h"
#include "TimeWindowsConstraint.h"

namespace Scheduler
{

	const TimeWindowsConstraint * OperationConstraints::timeWindows() const
	{
		return _time_windows.get();
	}

	const Demand * OperationConstraints::demand() const
	{
		return _demand.get();
	}

	void OperationConstraints::addConstraint(const TimeWindowsConstraint & time_windows)
	{
		this->_time_windows = std::make_unique<TimeWindowsConstraint>(time_windows);
	}

	void OperationConstraints::addConstraint(const Demand & demand)
	{
		this->_demand = std::make_unique<Demand>(demand);
	}
	void OperationConstraints::removeTimeWindowsConstraint()
	{
		_time_windows.reset();
	}
	void OperationConstraints::removeDemandConstraint()
	{
		_demand.reset();
	}
}