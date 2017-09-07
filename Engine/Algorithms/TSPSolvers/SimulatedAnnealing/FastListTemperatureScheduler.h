#pragma once

#include "ListTemperatureScheduler.h"

namespace Scheduler
{

	class FastListTemperatureScheduler : public ListTemperatureScheduler
	{
	public:
		FastListTemperatureScheduler( );
		FastListTemperatureScheduler(std::size_t list_size, std::size_t max_iterations_count, float initial_probability, float quality = 0.f);
		virtual FastListTemperatureScheduler* clone( ) const override;
		static constexpr const char* staticGetName( )
		{
			return "List-Fast";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}

	protected:
		virtual float new_value( ) override;

	private:
		const float first_ratio;
	};
}
