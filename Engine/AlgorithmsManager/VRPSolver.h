#pragma once

#include "Algorithm.h"

namespace Scheduler
{
	class Scene;

	class VRPSolver : public Algorithm
	{
	public:
		virtual void optimize(Scene& scene) const = 0;
	};
}