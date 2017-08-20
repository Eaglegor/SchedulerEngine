#pragma once

#include "Operation.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT WorkOperation : public Operation
	{
	public:
		WorkOperation(std::size_t id, const Location& location, const Order& order);

		const Location& getLocation( ) const;

	private:
		const Location& location;
	};
}
