#pragma once

#include <Engine/Strategies/VRPSolver.h>
#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT TransparentVRPSolver : public VRPSolver
	{
	public:
		virtual void optimize(Scene* scene) override;
	};
}