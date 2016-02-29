#pragma once

namespace Scheduler
{
	class RoutingService;

	class LoggingService;

	struct Context
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