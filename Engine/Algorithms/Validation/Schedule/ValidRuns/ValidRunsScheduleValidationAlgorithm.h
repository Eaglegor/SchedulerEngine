#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>


namespace Scheduler
{
	class ValidRunsScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual void validate(const Schedule& schedule, ViolationsConsumer& violations_consumer) const override;

		constexpr static const char* staticGetName( )
		{
			return "ValidRuns";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}
	};
}