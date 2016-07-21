#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Extensions/ScheduleValidationAlgorithm.h>
#include <DistanceLimitScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class DISTANCELIMITSCHEDULEVALIDATIONALGORITHM_EXPORT DistanceLimitScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		DistanceLimitScheduleValidationAlgorithm() = default;

		virtual bool isValid(const Schedule * run) const override;
	};
}