#pragma once

#include <Engine/SceneManager/Extensions/ScheduleValidationAlgorithm.h>
#include <ValidRunsScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class VALIDRUNSSCHEDULEVALIDATIONALGORITHM_EXPORT ValidRunsScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule * schedule) const override;
	};
}