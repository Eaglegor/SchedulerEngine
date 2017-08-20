#pragma once

#include "Operation.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT DepotOperation : public Operation
	{
	public:
		DepotOperation(std::size_t id, const Order& order);
	};
}
