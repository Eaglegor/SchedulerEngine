#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/SceneManager/Extensions/RunValidationAlgorithm.h>
#include <DistanceLimitRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class DISTANCELIMITRUNVALIDATIONALGORITHM_EXPORT DistanceLimitRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		explicit DistanceLimitRunValidationAlgorithm(const Distance &limit);
		virtual bool isValid(const Run * run) const override;

	private:
		Distance limit;
	};
}