#include "DepotStop.h"

#include <Engine/SceneManager/Operation.h>
#include <assert.h>

#include "ArrivalTimeActualizer.h"
#include "ConstStopVisitor.h"
#include "DepotOperation.h"
#include "DurationActualizer.h"
#include "StopVisitor.h"
#include <Engine/SceneManager/Location.h>
#include <Engine/Utils/Collections/Algorithms.h>

namespace Scheduler
{
	DepotStop::DepotStop(const Stop::Context& context, const Location& location, Run& run)
	    : Stop(context, run),
	      location(location)
	{
	}

	void DepotStop::addOperation(const DepotOperation& operation)
	{
		operations.emplace(operation);
		context.arrival_time_actualizer.setDirty(true);
		context.duration_actualizer.setDirty(true);
	}

	void DepotStop::removeOperation(const DepotOperation& operation)
	{
		operations.erase(operation);
		context.arrival_time_actualizer.setDirty(true);
		context.duration_actualizer.setDirty(true);
	}

	bool DepotStop::containsOperation(const DepotOperation& operation) const
	{
		return util::contains_key(operations, operation);
	}

	const Location& DepotStop::getLocation( ) const
	{
		return location;
	}

	const DepotStop::OperationsSet& DepotStop::getOperations( ) const
	{
		return operations;
	}

	void DepotStop::acceptVisitor(StopVisitor& visitor)
	{
		visitor.dispatch(*this);
	}

	void DepotStop::acceptVisitor(ConstStopVisitor& visitor) const
	{
		visitor.dispatch(*this);
	}
}
