#pragma once


#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <vector>

namespace Scheduler
{
	class BestOfTSPSolver : public TSPSolver
	{
	public:
		BestOfTSPSolver( );

		virtual void optimize(Schedule& schedule) const override;
		virtual void optimize(Run& schedule) const override;

		void addTSPSolver(const TSPSolver& aTSPSolver);
		void setScheduleCostFunction(const ScheduleCostFunction& cost_function);

		void setConcurrencyEnabled(bool value);

		static constexpr const char* staticGetName( )
		{
			return "BestOf";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

	private:
		virtual void concurrentOptimize(Schedule& schedule) const;
		virtual void concurrentOptimize(Run& schedule) const;
		virtual void sequentialOptimize(Schedule& schedule) const;
		virtual void sequentialOptimize(Run& schedule) const;

		std::vector<ReferenceWrapper<const TSPSolver>> tsp_solvers;
		Optional<const ScheduleCostFunction&> schedule_cost_function;
		bool concurrency_enabled;
		Logger& logger;
	};
}
