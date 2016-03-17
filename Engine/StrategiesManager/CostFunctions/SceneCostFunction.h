#pragma once

#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Scene;

    class SceneCostFunction
    {
        public:
			virtual Cost calculateCost(const Scene* scene) = 0;
	};
}