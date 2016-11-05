#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <ValidStopsRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class VALIDSTOPSRUNVALIDATIONALGORITHM_EXPORT ValidStopsRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual void validate(const Run& run, ViolationsConsumer& violations_consumer) const override;
		
		constexpr static const char* staticGetName(){return "ValidStops";}
		virtual const char* getName() const override{return staticGetName();}
	};
}