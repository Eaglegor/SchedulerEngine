#include "RunBoundaryStop.h"

#include <Engine/Utils/Collections/Algorithms.h>
#include "StopVisitor.h"

namespace Scheduler
{
	RunBoundaryStop::RunBoundaryStop(const Location & location, Run * run):
		Stop(location, run)
	{
	}

	void RunBoundaryStop::addOperation(const Operation * operation)
	{
		operations.emplace(operation);
	}

	void RunBoundaryStop::removeOperation(const Operation * operation)
	{
		operations.erase(operation);
	}

	bool RunBoundaryStop::containsOperation(const Operation * operation) const
	{
		return std::contains_key(operations, operation);
	}

	ImmutableUnorderedSet<const Operation*> RunBoundaryStop::getOperations() const
	{
		return operations;
	}

	void RunBoundaryStop::acceptVisitor(StopVisitor * visitor)
	{
		visitor->dispatch(this);
	}
	
}