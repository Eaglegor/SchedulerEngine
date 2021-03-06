#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/Routing/RoutingService.h>
#include <Engine/Utils/Optional.h>


namespace Scheduler
{
	class GreedyTSPSolver : public TSPSolver
	{
	public:
		GreedyTSPSolver( );

		virtual void optimize(Schedule& schedule) const override;
		virtual void optimize(Run& schedule) const override;

		void setRoutingService(const RoutingService& routing_service);

		static constexpr const char* staticGetName( )
		{
			return "Greedy";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

	private:
		Optional<const RoutingService&> routing_service;
		Logger& logger;
	};
}
