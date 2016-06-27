#pragma once

#include "EdgeSuggestor.h"
#include <vector>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Distance.h>

namespace Scheduler
{
	class RoutingService;
	class Run;

	class GCBIEdgeSuggestor : public EdgeSuggestor
	{
	public:
		GCBIEdgeSuggestor(Run* run, RoutingService* routing_service);

		virtual std::vector<SuggestedEdge> next() override;
		virtual bool hasNext() override;
		virtual void reset() override;

	private:
		struct Entry
		{
			Location from;
			Location to;
			Distance distance;
		};

		using Rating = std::vector<Entry>;

		std::vector<size_t> findStopsByLocation(const Location& location) const;

		void generateRating();

		Run* run;
		RoutingService* routing_service;
		Rating rating;
		Rating::iterator current_iterator;
	};
}
