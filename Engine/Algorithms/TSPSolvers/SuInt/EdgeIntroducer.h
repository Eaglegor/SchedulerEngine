#pragma once

#include "SuggestedEdge.h"

namespace Scheduler
{
	class SceneEditor;

	class EdgeIntroducer
	{
	public:
		virtual ~EdgeIntroducer(){}

		virtual bool introduce(const SuggestedEdge& edge) = 0;
	};
}