#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ScheduleValidationAlgorithm.h>
#include <ValidRunsScheduleValidationAlgorithm_export.h>

namespace Scheduler
{
	class VALIDRUNSSCHEDULEVALIDATIONALGORITHM_EXPORT ValidRunsScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule& schedule) const override;
		
		constexpr static const char* staticGetName(){return "ValidRuns";}
		virtual const char* getName() const override{return staticGetName();}
	};
}