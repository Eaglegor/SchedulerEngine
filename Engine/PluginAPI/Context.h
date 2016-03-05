#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class RoutingService;

	class LoggingService;

	struct SCHEDULERENGINE_EXPORT Context
	{
	public:
		Context() :
				routing_service(nullptr),
				logging_service(nullptr)
		{ }

		RoutingService *routing_service;
		LoggingService *logging_service;
	};
}