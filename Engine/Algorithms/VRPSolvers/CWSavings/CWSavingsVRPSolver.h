#pragma once

#include <Engine/StrategiesManager/VRPSolver.h>
#include <CWSavingsVRPSolver_export.h>

namespace Scheduler
{
	class Order;
	class Stop;
	
	class CWSAVINGSVRPSOLVER_EXPORT CWSavingsVRPSolver : public VRPSolver
	{
	public:
		void optimize(Scene* scene) const;
		static const char* staticGetName() {return "CWSavings";}
		const char* getName() const {return staticGetName();}
		
	private:
		bool performAction(Scene* scene, Order* i, Order* j) const;
	};
}