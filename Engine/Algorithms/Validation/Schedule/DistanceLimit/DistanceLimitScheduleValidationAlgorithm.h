#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Algorithms/Validation/ScheduleValidationAlgorithm.h>
#include <DistanceLimitScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class DISTANCELIMITSCHEDULEVALIDATIONALGORITHM_EXPORT DistanceLimitScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule& schedule) const override;
		
		constexpr static const char* staticGetName(){return "DistanceLimit";}
		virtual const char* getName() const override{return staticGetName();}
	};
}