#pragma once

#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Extensions/RunValidationAlgorithm.h>
#include <DurationLimitRunValidationAlgorithm_export.h>

namespace Scheduler
{
	class DURATIONLIMITRUNVALIDATIONALGORITHM_EXPORT DurationLimitRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		explicit DurationLimitRunValidationAlgorithm(const Duration &limit);
		virtual bool isValid(const Run * run) const override;

	private:
		Duration limit;
	};
}