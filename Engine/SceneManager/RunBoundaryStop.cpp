#include "RunBoundaryStop.h"

#include <Engine/SceneManager/Operation.h>
#include <assert.h>

#include <Engine/Utils/Collections/Algorithms.h>
#include "StopVisitor.h"
#include "ConstStopVisitor.h"
#include "ArrivalTimeActualizer.h"
#include "DurationActualizer.h"
#include <Engine/SceneManager/Location.h>

namespace Scheduler
{
	RunBoundaryStop::RunBoundaryStop(const Stop::Context& context, const Location& location, Run &run):
		Stop(context, run),
		location(location)
	{
	}

	void RunBoundaryStop::addOperation(const Operation& operation)
	{
		assert(location == operation.getLocation());
		operations.emplace(operation);
		context.arrival_time_actualizer.setDirty(true);
		context.duration_actualizer.setDirty(true);
	}

	void RunBoundaryStop::removeOperation(const Operation& operation)
	{
		operations.erase(operation);
		context.arrival_time_actualizer.setDirty(true);
		context.duration_actualizer.setDirty(true);
	}

	bool RunBoundaryStop::containsOperation(const Operation &operation) const
	{
		return util::contains_key(operations, operation);
	}

	const Location& RunBoundaryStop::getLocation() const
	{
		return location;
	}

	const RunBoundaryStop::OperationsSet& RunBoundaryStop::getOperations() const
	{
		return operations;
	}

	void RunBoundaryStop::acceptVisitor(StopVisitor& visitor)
	{
		visitor.dispatch(*this);
	}

	void RunBoundaryStop::acceptVisitor(ConstStopVisitor& visitor) const
	{
		visitor.dispatch(*this);
	}
}
