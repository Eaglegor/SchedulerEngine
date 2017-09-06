#pragma once


#include <Engine/SceneManager/Algorithms/Validation/ValidationAlgorithm.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <list>

namespace Scheduler
{
	class ChainScheduleValidationAlgorithm : public ScheduleValidationAlgorithm
	{
	public:
		virtual void validate(const Schedule& schedule, ViolationsConsumer& violations_consumer) const override;

		virtual void addAlgorithm(const ScheduleValidationAlgorithm& algorithm);

		constexpr static const char* staticGetName( )
		{
			return "Chain";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		}

	private:
		std::list<ReferenceWrapper<const ScheduleValidationAlgorithm>> algorithms;
	};
}