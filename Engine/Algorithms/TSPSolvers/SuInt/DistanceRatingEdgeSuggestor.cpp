#include "DistanceRatingEdgeSuggestor.h"

#include <Engine/Routing/RoutingService.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/CallableVisitorProxy.h>
#include <unordered_set>

namespace Scheduler
{
	DistanceRatingEdgeSuggestor::DistanceRatingEdgeSuggestor(Run& run, const RoutingService& routing_service)
	    : run(run),
	      routing_service(routing_service),
	      logger(LoggingManager::getLogger("DistanceRatingEdgeSuggestor"))
	{
		generateRating( );
		current_iterator = rating.begin( );

		Distance max_distance;
		for(Stop& stop : run.getStops( ))
		{
			if(stop.getNextRoute( ).getDistance( ) > max_distance) max_distance = stop.getNextRoute( ).getDistance( );
		}
		max_run_distance = max_distance;
	}

	std::vector<SuggestedEdge> DistanceRatingEdgeSuggestor::next( )
	{
		assert(current_iterator != rating.end( ));

		std::vector<SuggestedEdge> edges;

		const Entry rating_entry = *(current_iterator++);

		LOG_DEBUG(logger, "Selected edge: ({},{}) -> ({},{}) [Distance: {}]",
		          rating_entry.from.getLatitude( ).getValue( ),
		          rating_entry.from.getLongitude( ).getValue( ),
		          rating_entry.to.getLatitude( ).getValue( ),
		          rating_entry.to.getLongitude( ).getValue( ),
		          rating_entry.distance.getValue( ));

		std::vector<std::size_t> from_locations = findStopsBySite(rating_entry.from);

		LOG_TRACE(logger, "Matching 'from' stops count: {}", from_locations.size( ));

		std::vector<std::size_t> to_locations = findStopsBySite(rating_entry.to);

		LOG_TRACE(logger, "Matching 'to' stops count: {}", to_locations.size( ));

		for(std::size_t from_index : from_locations)
		{
			for(std::size_t to_index : to_locations)
			{
				if(from_index > 0 && from_index < run.getWorkStops( ).size( ) - 1)
				{
					if((std::next(run.getWorkStops( ).begin( ), from_index - 1))->getNextRoute( ).getDistance( ) < rating_entry.distance) continue;
				}
				else
				{
					if(run.getStartStop( ).getNextRoute( ).getDistance( ) < rating_entry.distance) continue;
				}

				SuggestedEdge edge;
				edge.from_index = from_index;
				edge.to_index   = to_index;
				edges.push_back(edge);
			}
		}

		return edges;
	}

	bool DistanceRatingEdgeSuggestor::hasNext( )
	{
		if(current_iterator->distance > max_run_distance)
		{
			LOG_DEBUG(logger, "Current edge distance is greater than the maximal distance in the run. Skipping remaining suggestions.");
			current_iterator = rating.end( );
		}

		return current_iterator != rating.end( );
	}

	void DistanceRatingEdgeSuggestor::reset( )
	{
		current_iterator = rating.begin( );
	}

	void DistanceRatingEdgeSuggestor::onSolutionAccepted( )
	{
		TRACEABLE_SECTION(__on_solution_accepted__, "Updating run max distance", logger);

		Distance max_distance;
		for(Stop& stop : run.getStops( ))
		{
			if(stop.getNextRoute( ).getDistance( ) > max_distance) max_distance = stop.getNextRoute( ).getDistance( );
		}
		max_run_distance = max_distance;
	}

	std::vector<std::size_t> DistanceRatingEdgeSuggestor::findStopsBySite(const Site& location) const
	{
		std::vector<std::size_t> result;

		if(run.getStartStop( ).getLocation( ).getSite( ) == location) result.push_back(0);

		std::size_t index = 1;
		for(const WorkStop& stop : run.getWorkStops( ))
		{
			if(stop.getLocation( ).getSite( ) == location) result.push_back(index);
			++index;
		}

		if(run.getEndStop( ).getLocation( ).getSite( ) == location) result.push_back(index);

		return result;
	}

	void DistanceRatingEdgeSuggestor::generateRating( )
	{
		rating.clear( );

		std::vector<Site> locations;

		for(Stop& stop : run.getStops( ))
		{
			locations.push_back(stop.getLocation( ).getSite( ));
		}

		locations.erase(std::unique(locations.begin( ), locations.end( )), locations.end( ));

		for(const Site& l1 : locations)
		{
			for(const Site& l2 : locations)
			{
				Entry edge;
				edge.from     = l1;
				edge.to       = l2;
				edge.distance = routing_service.calculateRoute(l1, l2, run.getVehicle( )->getRoutingProfile( )).getDistance( );
				rating.push_back(edge);
			}
		}

		auto comparator = [](const Entry& lhs, const Entry& rhs) { return lhs.distance < rhs.distance; };
		std::sort(rating.begin( ), rating.end( ), comparator);
	}
}
