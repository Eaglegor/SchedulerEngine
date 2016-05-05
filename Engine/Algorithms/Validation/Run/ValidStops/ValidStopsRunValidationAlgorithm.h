#pragma once

#include <Engine/SceneManager/Extensions/RunValidationAlgorithm.h>
#include <ValidStopsRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class VALIDSTOPSRUNVALIDATIONALGORITHM_EXPORT ValidStopsRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run * run) const override;
	};
}