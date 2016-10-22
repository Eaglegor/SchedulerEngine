#pragma once

#include <Engine/SceneManager/Algorithms/Validation/RunValidationAlgorithm.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <ChainRunValidationAlgorithm_export.h>
#include <list>

namespace Scheduler
{
	class CHAINRUNVALIDATIONALGORITHM_EXPORT ChainRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual bool isValid(const Run& run) const override;
		
		constexpr static const char* staticGetName(){return "Chain";}
		virtual const char* getName() const override{return staticGetName();}

		virtual void addAlgorithm(const RunValidationAlgorithm& algorithm);

	private:
		std::list<ReferenceWrapper<const RunValidationAlgorithm>> algorithms;
	};
}