#include "WorkStop.h"

#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/Operation.h>

#include "StopVisitor.h"
#include "ConstStopVisitor.h"

namespace Scheduler
{
	WorkStop::WorkStop(size_t id, Run* run) :
		Stop(run),
		id(id),
		operation(nullptr)
	{}

	const Operation * WorkStop::getOperation() const
	{
		return operation;
	}

	void WorkStop::setOperation(const Operation * operation)
	{
		this->operation = operation;
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		if(duration_actualizer) duration_actualizer->setDirty(true);
	}

	const Location& WorkStop::getLocation() const
	{
		assert(operation);
		return operation->getLocation();
	}

	void WorkStop::acceptVisitor(StopVisitor * visitor)
	{
		visitor->dispatch(this);
	}

	void WorkStop::acceptVisitor(ConstStopVisitor* visitor) const
	{
		visitor->dispatch(this);
	}
}
