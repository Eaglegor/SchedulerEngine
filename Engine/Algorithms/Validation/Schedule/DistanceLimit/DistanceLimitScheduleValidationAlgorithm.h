#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Extensions/ScheduleValidationAlgorithm.h>
#include <DistanceLimitScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class DISTANCELIMITSCHEDULEVALIDATIONALGORITHM_EXPORT DistanceLimitScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		explicit DistanceLimitScheduleValidationAlgorithm(const Distance &limit);
		virtual bool isValid(const Schedule * run) const override;

	private:
		Distance limit;
	};
}