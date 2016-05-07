#pragma once

#include <Engine/SceneManager/Extensions/RunValidationAlgorithm.h>
#include <OverloadRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class OVERLOADRUNVALIDATIONALGORITHM_EXPORT OverloadRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run * run) const override;
	};
}