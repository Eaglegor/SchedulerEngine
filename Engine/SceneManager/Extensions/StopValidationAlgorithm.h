#pragma once

namespace Scheduler
{
	class Stop;
	
	class StopValidationAlgorithm
	{
		public:
			virtual bool isValid(const Stop* stop) const = 0;
	};
}