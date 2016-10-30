#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Algorithms/Validation/RunValidationAlgorithm.h>
#include <DistanceLimitRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class DISTANCELIMITRUNVALIDATIONALGORITHM_EXPORT DistanceLimitRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run& run) const override;
		
		constexpr static const char* staticGetName(){return "DistanceLimit";}
		virtual const char* getName() const override{return staticGetName();}
	};
}