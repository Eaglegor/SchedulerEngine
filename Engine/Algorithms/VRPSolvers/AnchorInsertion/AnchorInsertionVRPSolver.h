#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Utils/Optional.h>
#include <AnchorInsertionVRPSolver.h>
#include <cstdint>
#include <memory>
#include "Engine/Engine/Services/RoutingService.h"

namespace Scheduler
{
	class TSPSolver;
	class SceneCostFunction;

    class ANCHORINSERTIONVRPSOLVER_EXPORT AnchorInsertionVRPSolver : public VRPSolver
	{
	public:
        AnchorInsertionVRPSolver();
        ~AnchorInsertionVRPSolver();
		virtual void optimize(Scene& scene) const override;

		static constexpr const char* staticGetName() { return "AnchorInsertion"; }
		virtual const char* getName() const override { return staticGetName(); };
		
		void setRepairAlgorithm(const VRPSolver& repair_algorithm);
		void setCostFunction(const SceneCostFunction& cost_function);
		void setIterationsLimit(std::size_t iterations_limit);
		void setRoutingService(const RoutingService& routing_service);

	private:
		Optional<const VRPSolver&> repair_algorithm;
		Optional<const SceneCostFunction&> cost_function;
		Optional<const RoutingService&> routing_service;
		std::size_t iterations_limit;
	};
}
