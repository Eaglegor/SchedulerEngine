#include "RunBoundaryStop.h"

#include <Engine/SceneManager/Operation.h>
#include <assert.h>

#include <Engine/Utils/Collections/Algorithms.h>
#include "StopVisitor.h"
#include "ConstStopVisitor.h"

namespace Scheduler
{
	RunBoundaryStop::RunBoundaryStop(const Location & location, Run * run):
		Stop(run),
		location(location)
	{
	}

	void RunBoundaryStop::addOperation(const Operation * operation)
	{
		assert(location == operation->getLocation());
		operations.emplace(operation);
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		if(duration_actualizer) duration_actualizer->setDirty(true);
	}

	void RunBoundaryStop::removeOperation(const Operation * operation)
	{
		operations.erase(operation);
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		if(duration_actualizer) duration_actualizer->setDirty(true);
	}

	bool RunBoundaryStop::containsOperation(const Operation * operation) const
	{
		return std::contains_key(operations, operation);
	}

	const Location& RunBoundaryStop::getLocation() const
	{
		return location;
	}

	ImmutableUnorderedSet<const Operation*> RunBoundaryStop::getOperations() const
	{
		return operations;
	}

	void RunBoundaryStop::acceptVisitor(StopVisitor * visitor)
	{
		visitor->dispatch(this);
	}

	void RunBoundaryStop::acceptVisitor(ConstStopVisitor* visitor) const
	{
		visitor->dispatch(this);
	}
}
