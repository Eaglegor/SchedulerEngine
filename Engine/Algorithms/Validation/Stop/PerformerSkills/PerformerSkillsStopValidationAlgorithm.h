#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <PerformerSkillsStopValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class PERFORMERSKILLSSTOPVALIDATIONALGORITHM_EXPORT PerformerSkillsStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual void validate(const Stop& stop, ViolationsConsumer& violations_consumer) const override;
		
		constexpr static const char* staticGetName(){return "PerformerSkills";}
		virtual const char* getName() const override{return staticGetName();}
	};
}