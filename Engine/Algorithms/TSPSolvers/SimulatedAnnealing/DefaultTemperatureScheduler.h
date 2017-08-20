#pragma once

#include "TemperatureScheduler.h"
#include <Engine/Concepts/Cost.h>
#include <cstddef>
#include <vector>

namespace Scheduler
{
	class Cost;

	class DefaultTemperatureScheduler : public TemperatureScheduler
	{
	public:
		DefaultTemperatureScheduler(float p0, float p_end, std::size_t iterations_number);
		virtual float getTemperature( ) const override;
		virtual bool isFinish( ) const override;
		static constexpr const char* staticGetName( )
		{
			return "Default";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}

		virtual void adapt(Cost delta, float random) override
		{
		}
		virtual void changeTemperature( ) override;
		virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) override;

		virtual TemperatureScheduler* clone( ) const override;

	private:
		const float p0;
		const float p_end;
		std::size_t iterations_number;
		float current_temperature;
		float k;
	};
}
