#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Utils/Optional.h>
#include <cstdint>
#include <memory>

namespace Scheduler
{

	class Renderer;
	class TSPSolver;
	class SceneCostFunction;
	class RoutingService;

	class AnchorInsertionVRPSolver : public VRPSolver
	{
	public:
		AnchorInsertionVRPSolver( );
		~AnchorInsertionVRPSolver( );
		virtual void optimize(Scene& scene) const override;

		static constexpr const char* staticGetName( )
		{
			return "AnchorInsertion";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

		void setRepairAlgorithm(const VRPSolver& repair_algorithm);
		void setCostFunction(const SceneCostFunction& cost_function);
		void setIterationsLimit(std::size_t iterations_limit);
		void setRoutingService(const RoutingService& routing_service);
		void setDebugRenderer(Renderer& renderer);

	private:
		Optional<const VRPSolver&> repair_algorithm;
		Optional<const SceneCostFunction&> cost_function;
		Optional<const RoutingService&> routing_service;
		Optional<Renderer&> debug_renderer;
		std::size_t iterations_limit;
	};
}
