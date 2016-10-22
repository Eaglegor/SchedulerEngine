#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Engine/Services/RoutingService.h>
#include <Engine/Utils/Optional.h>
#include <CWSavingsVRPSolver_export.h>

namespace Scheduler
{
	class Order;
	class Stop;
	class SceneEditor;
	
	class CWSAVINGSVRPSOLVER_EXPORT CWSavingsVRPSolver : public VRPSolver
	{
	public:
		void optimize(Scene& scene) const;
		
		static const char* staticGetName() {return "CWSavings";}
		const char* getName() const {return staticGetName();}
		
		void setRoutingService(const RoutingService& routing_service);
		
	private:
		bool performAction(Scene& scene, SceneEditor& editor, const Order& i, const Order& j) const;
		
		Optional<const RoutingService&> routing_service;
	};
}