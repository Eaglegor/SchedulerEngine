#pragma once

#include <Engine/StrategiesManager/VRPSolver.h>
#include <Engine/Engine/Services/RoutingService.h>
#include <CWSavingsVRPSolver_export.h>

namespace Scheduler
{
	class Order;
	class Stop;
	class SceneEditor;
	
	class CWSAVINGSVRPSOLVER_EXPORT CWSavingsVRPSolver : public VRPSolver
	{
	public:
		void optimize(Scene* scene) const;
		static const char* staticGetName() {return "CWSavings";}
		const char* getName() const {return staticGetName();}
		
		void setRoutingService(RoutingService* routing_service);
		
	private:
		bool performAction(Scene* scene, SceneEditor& editor, Order* i, Order* j) const;
		
		RoutingService* routing_service;
	};
}