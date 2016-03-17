#pragma once

#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>

#include <TotalDistanceCostFunctions_export.h>

namespace Scheduler
{
	class TOTALDISTANCECOSTFUNCTIONS_EXPORT TotalDistanceScheduleCostFunction : public ScheduleCostFunction
	{
	public:
		virtual Cost calculateCost(const Schedule* schedule) override;
	};
}