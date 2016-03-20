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

	Route TspLibRoutingService::calculateRoute(const Location& from, const Location& to, const RoutingProfile& routing_profile)
	{
		assert(from.getLatitude().getValue() >= -0.4);
		assert(to.getLatitude().getValue() >= -0.4);

		size_t from_index = std::round(from.getLatitude().getValue());
		size_t to_index = std::round(to.getLatitude().getValue());

		float distance = routes[from_index * nodes_count + to_index];

		if(distance < FLOAT_EPSILON)
		{
			distance = 100500.0f; // Constant large enough to indicate that the route is not feasible
		}

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
