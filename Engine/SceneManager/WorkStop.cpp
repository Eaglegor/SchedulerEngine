#include "WorkStop.h"

#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/Operation.h>

#include "StopVisitor.h"

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
}