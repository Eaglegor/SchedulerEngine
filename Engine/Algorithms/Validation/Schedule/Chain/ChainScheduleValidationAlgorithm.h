#pragma once

#include <Engine/SceneManager/Algorithms/Validation/ScheduleValidationAlgorithm.h>
#include <ChainScheduleValidationAlgorithm_export.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <list>

namespace Scheduler
{
	class CHAINSCHEDULEVALIDATIONALGORITHM_EXPORT ChainScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual bool isValid(const Schedule& schedule) const override;

		virtual void addAlgorithm(const ScheduleValidationAlgorithm& algorithm);

		constexpr static const char* staticGetName(){return "Chain";}
		virtual const char* getName() const{return staticGetName();}
		
	private:
		std::list<ReferenceWrapper<const ScheduleValidationAlgorithm>> algorithms;
	};
}