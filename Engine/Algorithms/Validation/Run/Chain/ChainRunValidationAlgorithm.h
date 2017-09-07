#pragma once


#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <list>

namespace Scheduler
{
	class ChainRunValidationAlgorithm : public RunValidationAlgorithm
	{
	public:
		virtual void validate(const Run& run, ViolationsConsumer& violations_consumer) const override;

		constexpr static const char* staticGetName( )
		{
			return "Chain";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}

		virtual void addAlgorithm(const RunValidationAlgorithm& algorithm);

	private:
		std::list<ReferenceWrapper<const RunValidationAlgorithm>> algorithms;
	};
}