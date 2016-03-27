#pragma once

#include <Engine/Engine/Services/RoutingService.h>
#include <unordered_map>
#include <functional>

#include <TspLibRoutingService_export.h>

namespace Scheduler
{
	class TSPLIBROUTINGSERVICE_EXPORT TspLibRoutingService : public RoutingService
	{
	public:
		TspLibRoutingService();
		~TspLibRoutingService();
		
		void init(size_t nodes_count);

		virtual Route calculateRoute(const Location& from, const Location& to, const RoutingProfile& routing_profile) override;

		void insertRoute(size_t ia, size_t ib, float distance);

	private:
		float* routes;
		size_t nodes_count;
	};
}