#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <VehicleRequirementsStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class VEHICLEREQUIREMENTSSTOPVALIDATIONALGORITHM_EXPORT VehicleRequirementsStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual void validate(const Stop& stop, ViolationsConsumer& violations_consumer) const override;

		constexpr static const char* staticGetName( )
		{
			return "VehicleRequirements";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}
	};
}