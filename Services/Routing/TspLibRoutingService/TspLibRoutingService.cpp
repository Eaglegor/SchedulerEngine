#include "TspLibRoutingService.h"

#include <assert.h>
#include <Engine/Concepts/Speed.h>
#include <Engine/Concepts/SpeedDurationDistanceOperations.h>
#include <Engine/Math/FloatEpsilon.h>

#include <cstring>

namespace Scheduler
{
	TspLibRoutingService::TspLibRoutingService():
		routes(nullptr),
		nodes_count(0)
	{
	}

	TspLibRoutingService::~TspLibRoutingService()
	{
		if(routes) delete[] routes;
	}

	void TspLibRoutingService::init(size_t nodes_count)
	{
		if (routes) delete[] routes;

		this->nodes_count = nodes_count;
		this->routes = new float[nodes_count * nodes_count];
		memset(this->routes, 0, sizeof(*routes) * nodes_count * nodes_count);
	}

	Route TspLibRoutingService::calculateRoute(const Site& from, const Site& to, const RoutingProfile& routing_profile)
	{
		size_t from_index = from.getLatitude().getValue();
		size_t to_index = to.getLatitude().getValue();
		
		float distance = routes[from_index * nodes_count + to_index];

		Route r;
		r.setDistance(Distance(distance));
		r.setDuration(Distance(distance) / Speed(Distance(60)));
		r.setFrom(from);
		r.setTo(to);

		return r;
	}

	void TspLibRoutingService::insertRoute(size_t ia, size_t ib, float distance)
	{
		assert(ia * nodes_count + ib < nodes_count * nodes_count);
		routes[ia * nodes_count + ib] = distance;
	}
}
