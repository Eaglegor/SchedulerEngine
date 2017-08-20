#include "DefaultTemperatureScheduler.h"
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{

	DefaultTemperatureScheduler::DefaultTemperatureScheduler(float ap0, float ap_end, std::size_t aiterations_number)
	    : p0(ap0), p_end(ap_end), iterations_number(aiterations_number)
	{
	}

	float DefaultTemperatureScheduler::getTemperature( ) const
	{
		return current_temperature;
	}

	bool DefaultTemperatureScheduler::isFinish( ) const
	{
		return iterations_number == 0 || current_temperature <= 0.f;
	}

	void DefaultTemperatureScheduler::changeTemperature( )
	{
		current_temperature *= k;
		if(iterations_number >= 0)
		{
			--iterations_number;
		}
	}

	void DefaultTemperatureScheduler::initialize(Run& run, const ScheduleCostFunction& cost_function)
	{
		const auto costs = create_initial_costs(run, cost_function, 120);
		Cost first_cost  = costs.front( );
		float sum_diff   = 0.f;
		for(auto it = std::next(costs.begin( )); it < costs.end( ); ++it)
		{
			sum_diff += fabs((first_cost - *it).getValue( ));
		}
		const float avr     = sum_diff / (costs.size( ) - 1);
		current_temperature = -avr / p0;
		k                   = std::pow(-avr / p_end / current_temperature, (float) 1 / iterations_number);
	}

	TemperatureScheduler* DefaultTemperatureScheduler::clone( ) const
	{
		return new DefaultTemperatureScheduler(*this);
	}
}
