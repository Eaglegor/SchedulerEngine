#pragma once

namespace Scheduler
{
	class Scene;

	class VRPSolver
	{
	public:
		virtual ~VRPSolver(){}

		virtual void optimize(Scene* scene) const = 0;
	};
}