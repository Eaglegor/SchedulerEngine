#pragma once

#include <Engine/SceneManager/Algorithms/Validation/StopValidationAlgorithm.h>
#include <VehicleRequirementsStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class VEHICLEREQUIREMENTSSTOPVALIDATIONALGORITHM_EXPORT VehicleRequirementsStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop& stop) const override;
		
		constexpr static const char* staticGetName(){return "VehicleRequirements";}
		virtual const char* getName() const override{return staticGetName();}
	};
}