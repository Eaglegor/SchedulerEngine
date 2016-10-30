#pragma once

#include "EdgeSuggestor.h"
#include <vector>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Distance.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <cstddef>

namespace Scheduler
{
	class RoutingService;
	class Run;

	class DistanceRatingEdgeSuggestor : public EdgeSuggestor
	{
	public:
		DistanceRatingEdgeSuggestor(Run& run, const RoutingService& routing_service);

		virtual std::vector<SuggestedEdge> next() override;
		virtual bool hasNext() override;
		virtual void reset() override;

		void onSolutionAccepted();
	private:
		struct Entry
		{
			Site from;
			Site to;
			Distance distance;
		};

		using Rating = std::vector<Entry>;

		std::vector<std::size_t> findStopsBySite(const Site& location) const;

		void generateRating();

		Run& run;
		const RoutingService& routing_service;
		Rating rating;
		Rating::iterator current_iterator;
		Logger& logger;
		Distance max_run_distance;
	};
}
