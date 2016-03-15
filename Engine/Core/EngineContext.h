#pragma once

#include <Engine_export.h>

namespace Scheduler
{
	class RoutingService;
	
	struct ENGINE_EXPORT EngineContext
	{
		RoutingService* routing_service;
		
		EngineContext():
		routing_service(nullptr)
		{}
	};
}