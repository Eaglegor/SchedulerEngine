#pragma once

#include "TemperatureScheduler.h"

#include <cstddef>
#include <queue>

namespace Scheduler
{
	class Cost;

	class ListTemperatureScheduler : public TemperatureScheduler
	{
	public:
		ListTemperatureScheduler( );
		ListTemperatureScheduler(std::size_t list_size, std::size_t iterations_limit, float initial_probability);
		virtual ListTemperatureScheduler* clone( ) const override;
		virtual float getTemperature( ) const override;
		virtual bool isFinish( ) const override;

		virtual void adapt(Cost delta, float random) override;
		virtual void changeTemperature( ) override;
		virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) override;

		static constexpr const char* staticGetName( )
		{
			return "List";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}

	protected:
		virtual float new_value( );

		std::vector<float> adapt_values;

	private:
		const std::size_t list_size;
		const std::size_t iterations_limit;
		const float initial_probability;
		std::priority_queue<float> values;
		std::size_t iteration_counter;
	};
}
