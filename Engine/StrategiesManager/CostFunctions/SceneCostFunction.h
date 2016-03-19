#pragma once

#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Scene;

    class SceneCostFunction
    {
        public:
	    virtual ~SceneCostFunction()
	    {
	    }

	    virtual Cost calculateCost(const Scene* scene) = 0;

		virtual const char* getName() const = 0;
	};
}