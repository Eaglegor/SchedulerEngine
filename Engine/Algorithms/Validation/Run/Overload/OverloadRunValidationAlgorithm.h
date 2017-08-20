#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <OverloadRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class OVERLOADRUNVALIDATIONALGORITHM_EXPORT OverloadRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual void validate(const Run& run, ViolationsConsumer& violations_consumer) const override;


		constexpr static const char* staticGetName( )
		{
			return "RunOverload";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}
	};
}