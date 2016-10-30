#include "WorkStop.h"

#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/Operation.h>

#include "StopVisitor.h"
#include "ConstStopVisitor.h"

namespace Scheduler
{
	WorkStop::WorkStop(std::size_t id, const Stop::Context& context, Run& run, const Operation& operation) :
		Stop(context, run),
		id(id),
		operation(operation)
	{}

	const Operation &WorkStop::getOperation() const
	{
		return operation;
	}

	const Location& WorkStop::getLocation() const
	{
		return operation.getLocation();
	}

	void WorkStop::acceptVisitor(StopVisitor& visitor)
	{
		visitor.dispatch(*this);
	}

	void WorkStop::acceptVisitor(ConstStopVisitor& visitor) const
	{
		visitor.dispatch(*this);
	}
	
	bool WorkStop::operator==(const WorkStop& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}
	
	bool WorkStop::operator!=(const WorkStop& rhs) const
	{
		return !(*this == rhs);
	}
	
}
