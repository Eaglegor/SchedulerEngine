#pragma once

#include <Engine/StrategiesManager/VRPSolver.h>
#include <TransparentVRPSolver_export.h>

namespace Scheduler
{
	class TRANSPARENTVRPSOLVER_EXPORT TransparentVRPSolver : public VRPSolver
	{
	public:
		virtual void optimize(Scene* scene) const override;

		static constexpr const char* staticGetName() { return "Transparent"; }
		virtual const char* getName() const override { return staticGetName(); };
	};
}