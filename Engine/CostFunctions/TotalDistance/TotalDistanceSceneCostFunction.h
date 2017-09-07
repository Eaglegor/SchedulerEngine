#pragma once

#include <Engine/SceneManager/CostFunctions/SceneCostFunction.h>



namespace Scheduler
{
	class TotalDistanceSceneCostFunction : public SceneCostFunction
	{
	public:
		virtual Cost calculateCost(const Scene& scene) const override;

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