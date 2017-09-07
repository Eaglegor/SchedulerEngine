#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/Utils/Optional.h>


namespace Scheduler
{
	class OneRelocateTSPSolver : public TSPSolver
	{
	public:
		OneRelocateTSPSolver( );

		virtual void optimize(Schedule& schedule) const override;
		virtual void optimize(Run& schedule) const override;

		void setScheduleCostFunction(const ScheduleCostFunction& cost_function);

		static constexpr const char* staticGetName( )
		{
			return "OneRelocate";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

	private:
		Optional<const ScheduleCostFunction&> schedule_cost_function;
		Logger& logger;
	};
}
