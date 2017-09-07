#pragma once


#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>

namespace Scheduler
{
	class DurationLimitRunValidationAlgorithm : public RunValidationAlgorithm
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