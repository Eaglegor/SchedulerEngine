#pragma once

#include <Engine/Routing/RoutingService.h>
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
		
		void init(std::size_t nodes_count);

		virtual Route calculateRoute(const Site& from, const Site& to, const RoutingProfile& routing_profile) const override;

		void insertRoute(std::size_t ia, std::size_t ib, float distance);

	private:
		float* routes;
		std::size_t nodes_count;
	};
}