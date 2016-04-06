#include "WorkStop.h"

#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/Operation.h>
#include "ScheduleActualizer.h"

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
		if (this->operation) schedule_actualizer->onOperationRemoved(this);
		this->operation = operation;
		if (operation) schedule_actualizer->onOperationAdded(this, operation);
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