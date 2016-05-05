#pragma once

#include <Engine/SceneManager/Extensions/StopValidationAlgorithm.h>
#include <PerformerSkillsStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class PERFORMERSKILLSSTOPVALIDATIONALGORITHM_EXPORT PerformerSkillsStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop * stop) const override;
	};
}