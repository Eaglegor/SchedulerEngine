#pragma once

#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Algorithms/Validation/RunValidationAlgorithm.h>
#include <DurationLimitRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class DURATIONLIMITRUNVALIDATIONALGORITHM_EXPORT DurationLimitRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run& run) const override;
		
		constexpr static const char* staticGetName(){return "DurationLimit";}
		virtual const char* getName() const override{return staticGetName();}
	};
}