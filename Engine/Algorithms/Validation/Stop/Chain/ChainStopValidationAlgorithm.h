#pragma once

#include <Engine/SceneManager/Algorithms/Validation/StopValidationAlgorithm.h>
#include <ChainStopValidationAlgorithm_export.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <list>

namespace Scheduler
{
	class CHAINSTOPVALIDATIONALGORITHM_EXPORT ChainStopValidationAlgorithm : public StopValidationAlgorithm
	{
	public:
		virtual bool isValid(const Stop& stop) const override;

		virtual void addAlgorithm(const StopValidationAlgorithm& algorithm);
		
		constexpr static const char* staticGetName(){return "Chain";}
		virtual const char* getName() const override{return staticGetName();}

	private:
		std::list<ReferenceWrapper<const StopValidationAlgorithm>> algorithms;
	};
}