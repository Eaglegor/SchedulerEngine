#pragma once


#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>

namespace Scheduler
{
	class DistanceLimitRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual void validate(const Run& run, ViolationsConsumer& violations_consumer) const override;

		constexpr static const char* staticGetName( )
		{
			return "DistanceLimit";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}
	};
}