#include "GCBIEdgeSuggestor.h"

#include <unordered_set>
#include <Engine/Engine/Services/RoutingService.h>
#include <Engine/Utils/CallableVisitorProxy.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	GCBIEdgeSuggestor::GCBIEdgeSuggestor(Run* run, RoutingService* routing_service):
		run(run),
		routing_service(routing_service)
	{
		generateRating();
		current_iterator = rating.begin();
	}

	std::vector<SuggestedEdge> GCBIEdgeSuggestor::next()
	{
		assert(current_iterator != rating.end());

		std::vector<SuggestedEdge> edges;
		
		const Entry rating_entry = *(current_iterator++);
		
		std::vector<size_t> from_locations = findStopsByLocation(rating_entry.from);
		std::vector<size_t> to_locations = findStopsByLocation(rating_entry.to);

		for(size_t from_index : from_locations)
		{
			for(size_t to_index : to_locations)
			{
				SuggestedEdge edge;
				edge.from_index = from_index;
				edge.to_index = to_index;
				edges.push_back(edge);
			}
		}
		
		return edges;
	}

	bool GCBIEdgeSuggestor::hasNext()
	{
		return current_iterator != rating.end();
	}

	void GCBIEdgeSuggestor::reset()
	{
		current_iterator = rating.begin();
	}

	std::vector<size_t> GCBIEdgeSuggestor::findStopsByLocation(const Location& location) const
	{
		std::vector<size_t> result;
		
		if (run->getStartStop()->getLocation() == location) result.push_back(0);

		size_t index = 1;
		for (const WorkStop* stop : run->getWorkStops())
		{
			if (stop->getLocation() == location) result.push_back(index);
			++index;
		}

		if (run->getEndStop()->getLocation() == location) result.push_back(index);

		return result;
	}

	void GCBIEdgeSuggestor::generateRating()
	{
		rating.clear();

		std::vector<Location> locations;

		for (Stop* stop = run->getStartStop(); stop != run->getEndStop()->getNextStop(); stop = stop->getNextStop())
		{
			locations.push_back(stop->getLocation());
		}

		locations.erase(std::unique(locations.begin(), locations.end()), locations.end());

		for(const Location& l1 : locations)
		{
			for (const Location& l2 : locations)
			{
				Entry edge;
				edge.from = l1;
				edge.to = l2;
				edge.distance = routing_service->calculateRoute(l1, l2, run->getVehicle()->getRoutingProfile()).getDistance();
				rating.push_back(edge);
			}
		}

		auto comparator = [](const Entry& lhs, const Entry& rhs) {return lhs.distance < rhs.distance; };
		std::sort(rating.begin(), rating.end(), comparator);
	}
}
