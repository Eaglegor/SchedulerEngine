#pragma once

#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>



namespace Scheduler
{
	class TotalDistanceScheduleCostFunction : public ScheduleCostFunction
	{
	public:
		virtual Cost calculateCost(const Schedule& schedule) const override;

		static constexpr const char* staticGetName( )
		{
			return "TotalDistance";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};
	};
}