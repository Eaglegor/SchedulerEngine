#pragma once

#include "EdgeSuggestor.h"
#include <vector>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Distance.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/SceneManager/Stop.h>

namespace Scheduler
{
	class RoutingService;
	class Run;

	class BetterEdgeSuggestor : public EdgeSuggestor
	{
	public:
		BetterEdgeSuggestor(Run* run, RoutingService* routing_service);

		virtual std::vector<SuggestedEdge> next() override;
		virtual bool hasNext() override;
		virtual void reset() override;

	private:
		struct Entry
		{
			SuggestedEdge edge;
			float distance;

			bool operator<(const Entry& rhs) const
			{
				return this->distance < rhs.distance;
			}
		};

		Run* run;
		RoutingService* routing_service;
		size_t current_index;
		Logger* logger;
	};
}
