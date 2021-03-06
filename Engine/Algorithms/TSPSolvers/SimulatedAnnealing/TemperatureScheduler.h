#pragma once

#include <cstddef>
#include <vector>

namespace Scheduler
{
	class Run;
	class ScheduleCostFunction;
	class Cost;

	class TemperatureScheduler
	{
	public:
		virtual ~TemperatureScheduler( )
		{
		}

		virtual float getTemperature( ) const = 0;
		virtual bool isFinish( ) const        = 0;
		virtual const char* getName( ) const  = 0;

		virtual void adapt(Cost delta, float random) = 0;
		virtual void changeTemperature( ) = 0;
		virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) = 0;

		virtual TemperatureScheduler* clone( ) const = 0;

	protected:
		std::vector<Cost> create_initial_costs(Run& run, const ScheduleCostFunction& cost_function, std::size_t length);
	};
}
