#pragma once

#include "SuggestedEdge.h"
#include <vector>

namespace Scheduler
{
	class EdgeSuggestor
	{
	public:
		virtual ~EdgeSuggestor(){}

		virtual std::vector<SuggestedEdge> next() = 0;
		virtual bool hasNext() = 0;
		virtual void reset() = 0;
	};
}