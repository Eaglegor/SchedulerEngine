#pragma once

#include <Engine/StrategiesManager/CostFunctions/RunCostFunction.h>

#include <TotalDistanceCostFunctions_export.h>

namespace Scheduler
{
	class TOTALDISTANCECOSTFUNCTIONS_EXPORT TotalDistanceRunCostFunction : public RunCostFunction
	{
	public:
		virtual Cost calculateCost(const Run* run) override;
	};
}