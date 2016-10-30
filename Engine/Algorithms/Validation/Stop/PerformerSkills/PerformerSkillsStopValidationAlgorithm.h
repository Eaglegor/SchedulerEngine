#pragma once

#include <Engine/SceneManager/Algorithms/Validation/StopValidationAlgorithm.h>
#include <PerformerSkillsStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class PERFORMERSKILLSSTOPVALIDATIONALGORITHM_EXPORT PerformerSkillsStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop& stop) const override;
		
		constexpr static const char* staticGetName(){return "PerformerSkills";}
		virtual const char* getName() const override{return staticGetName();}
	};
}