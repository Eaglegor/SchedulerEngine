#pragma once

#include <DurationLimitRunValidationAlgorithm_export.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>

namespace Scheduler
{
	class DURATIONLIMITRUNVALIDATIONALGORITHM_EXPORT DurationLimitRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual void validate(const Run& run, ViolationsConsumer& violations_consumer) const override;

		constexpr static const char* staticGetName( )
		{
			return "DurationLimit";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}
	};
}