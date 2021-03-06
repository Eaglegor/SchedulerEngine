#include "BetterEdgeSuggestor.h"

#include <Engine/Routing/RoutingService.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/CallableVisitorProxy.h>
#include <unordered_set>

namespace Scheduler
{
	BetterEdgeSuggestor::BetterEdgeSuggestor(Run& run, const RoutingService& routing_service)
	    : run(run),
	      routing_service(routing_service),
	      logger(LoggingManager::getLogger("BetterEdgeSuggestor")),
	      current_index(0)
	{
	}

	std::vector<SuggestedEdge> BetterEdgeSuggestor::next( )
	{
		assert(current_index < run.getWorkStops( ).size( ) - 1);

		Optional<Stop&> current_stop;
		if(current_index == 0)
			current_stop = run.getStartStop( );
		else
			current_stop = *std::next(run.getWorkStops( ).begin( ), current_index - 1);

		Distance current_distance = current_stop->getNextRoute( ).getDistance( );

		std::vector<Entry> entries;

		for(std::size_t i = 1; i <= run.getWorkStops( ).size( ) + 1; ++i)
		{
			if(i == current_index || i == current_index + 1) continue;

			Optional<Stop&> new_next_stop;
			if(i == run.getWorkStops( ).size( ) + 1)
				new_next_stop = run.getEndStop( );
			else
				new_next_stop = *std::next(run.getWorkStops( ).begin( ), i - 1);

			Distance new_distance = routing_service.calculateRoute(current_stop->getLocation( ).getSite( ), new_next_stop->getLocation( ).getSite( ), run.getVehicle( )->getRoutingProfile( )).getDistance( );
			if(new_distance < current_distance)
			{
				Entry entry;
				entry.edge.from_index = current_index;
				entry.edge.to_index   = i;
				entry.distance        = new_distance.getValue( );
				entries.push_back(entry);
			}
		}

		std::sort(entries.begin( ), entries.end( ));

		std::vector<SuggestedEdge> edges;
		for(const Entry& entry : entries)
		{
			edges.push_back(entry.edge);
		}

		++current_index;

		return edges;
	}

	bool BetterEdgeSuggestor::hasNext( )
	{
		return current_index < run.getWorkStops( ).size( ) - 1;
	}

	void BetterEdgeSuggestor::reset( )
	{
		current_index = 0;
	}
}
