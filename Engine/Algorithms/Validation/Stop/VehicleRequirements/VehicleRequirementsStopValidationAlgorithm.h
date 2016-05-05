#pragma once

#include <Engine/SceneManager/Extensions/StopValidationAlgorithm.h>
#include <VehicleRequirementsStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class VEHICLEREQUIREMENTSSTOPVALIDATIONALGORITHM_EXPORT VehicleRequirementsStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop * stop) const override;
	};
}