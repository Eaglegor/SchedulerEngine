#pragma once

namespace Scheduler
{
	class Stop;
	
	class StopValidationAlgorithm
	{
		public:
			virtual ~StopValidationAlgorithm(){}

			virtual bool isValid(const Stop* stop) const = 0;
	};
}