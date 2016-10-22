#pragma once

#include <Engine/SceneManager/Algorithms/Validation/RunValidationAlgorithm.h>
#include <OverloadRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class OVERLOADRUNVALIDATIONALGORITHM_EXPORT OverloadRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run& run) const override;
		
		constexpr static const char* staticGetName(){return "RunOverload";}
		virtual const char* getName() const override{return staticGetName();}
	};
}