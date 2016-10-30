#pragma once

#include <Engine/SceneManager/Algorithms/Validation/RunValidationAlgorithm.h>
#include <ValidStopsRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class VALIDSTOPSRUNVALIDATIONALGORITHM_EXPORT ValidStopsRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run& run) const override;
		
		constexpr static const char* staticGetName(){return "ValidStops";}
		virtual const char* getName() const override{return staticGetName();}
	};
}