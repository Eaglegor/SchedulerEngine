#pragma once

#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Extensions/ScheduleValidationAlgorithm.h>
#include <DurationLimitScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class DURATIONLIMITSCHEDULEVALIDATIONALGORITHM_EXPORT DurationLimitScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		explicit DurationLimitScheduleValidationAlgorithm(const Duration& limit);
		virtual bool isValid(const Schedule * run) const override;

	private:
		Duration limit;
	};
}