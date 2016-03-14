#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Scene;

	class SCHEDULERENGINE_EXPORT VRPSolver
	{
	public:
		virtual ~VRPSolver(){}

		virtual void optimize(Scene* scene) = 0;
	};
}