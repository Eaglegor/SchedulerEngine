#pragma once

#include <Engine/StrategiesManager/CostFunctions/SceneCostFunction.h>

#include <TotalDistanceCostFunctions_export.h>

namespace Scheduler
{
	class TOTALDISTANCECOSTFUNCTIONS_EXPORT TotalDistanceSceneCostFunction : public SceneCostFunction
	{
	public:
		virtual Cost calculateCost(const Scene* scene) override;

		static constexpr const char* staticGetName() { return "TotalDistance"; }
		virtual const char* getName() const override { return staticGetName(); };
	};
}