#include "DistanceRatingEdgeSuggestor.h"

#include <unordered_set>
#include <Engine/Engine/Services/RoutingService.h>
#include <Engine/Utils/CallableVisitorProxy.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	DistanceRatingEdgeSuggestor::DistanceRatingEdgeSuggestor(Run* run, RoutingService* routing_service):
		run(run),
		routing_service(routing_service),
		logger(LoggingManager::getLogger("GCBIEdgeSuggestor"))
	{
		generateRating();
		current_iterator = rating.begin();

		Distance max_distance;
		for (Stop* stop = run->getStartStop(); stop != run->getEndStop()->getNextStop(); stop = stop->getNextStop())
		{
			if (stop->getNextRoute().getDistance() > max_distance) max_distance = stop->getNextRoute().getDistance();
		}
		max_run_distance = max_distance;
	}

	std::vector<SuggestedEdge> DistanceRatingEdgeSuggestor::next()
	{
		assert(current_iterator != rating.end());

		std::vector<SuggestedEdge> edges;
		
		const Entry rating_entry = *(current_iterator++);

		LOG_DEBUG(logger, "Selected edge: ({},{}) -> ({},{}) [Distance: {}]", 
			rating_entry.from.getLatitude().getValue(), 
			rating_entry.from.getLongitude().getValue(),
			rating_entry.to.getLatitude().getValue(),
			rating_entry.to.getLongitude().getValue(),
			rating_entry.distance.getValue()
			);

		std::vector<size_t> from_locations = findStopsByLocation(rating_entry.from);
		LOG_TRACE(logger, "Matching 'from' stops count: {}", from_locations.size());

		std::vector<size_t> to_locations = findStopsByLocation(rating_entry.to);
		LOG_TRACE(logger, "Matching 'to' stops count: {}", to_locations.size());

		for(size_t from_index : from_locations)
		{
			for(size_t to_index : to_locations)
			{
				if (from_index > 0 && from_index < run->getWorkStops().size() - 1)
				{
					if ((*(run->getWorkStops().begin() + (from_index - 1)))->getNextRoute().getDistance() < rating_entry.distance) continue;
				}
				else
				{
					if (run->getStartStop()->getNextRoute().getDistance() < rating_entry.distance) continue;
				}

				SuggestedEdge edge;
				edge.from_index = from_index;
				edge.to_index = to_index;
				edges.push_back(edge);
			}
		}
		
		return edges;
	}

	bool DistanceRatingEdgeSuggestor::hasNext()
	{
		if (current_iterator->distance > max_run_distance)
		{
			SIMPLE_LOG_DEBUG(logger, "Current edge distance is greater than the maximal distance in the run. Skipping remaining suggestions.");
			current_iterator = rating.end();
		}

		return current_iterator != rating.end();
	}

	void DistanceRatingEdgeSuggestor::reset()
	{
		current_iterator = rating.begin();
	}

	void DistanceRatingEdgeSuggestor::onSolutionAccepted()
	{
		SIMPLE_LOG_TRACE(logger, "Updating run max distance");
		Distance max_distance;
		for (Stop* stop = run->getStartStop(); stop != run->getEndStop()->getNextStop(); stop = stop->getNextStop())
		{
			if (stop->getNextRoute().getDistance() > max_distance) max_distance = stop->getNextRoute().getDistance();
		}
		max_run_distance = max_distance;
	}

	std::vector<size_t> DistanceRatingEdgeSuggestor::findStopsByLocation(const Location& location) const
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

	void DistanceRatingEdgeSuggestor::generateRating()
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
