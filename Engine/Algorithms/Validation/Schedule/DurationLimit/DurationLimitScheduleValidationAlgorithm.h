#pragma once

#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Algorithms/Validation/ScheduleValidationAlgorithm.h>
#include <DurationLimitScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class DURATIONLIMITSCHEDULEVALIDATIONALGORITHM_EXPORT DurationLimitScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule& schedule) const override;
		
		constexpr static const char* staticGetName(){return "DurationLimit";}
		virtual const char* getName() const override{return staticGetName();}
	};
}