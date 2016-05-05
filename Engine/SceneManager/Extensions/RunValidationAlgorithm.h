#pragma once

namespace Scheduler
{
	class Run;
	
	class RunValidationAlgorithm
	{
		public:
			virtual bool isValid(const Run* run) const = 0;
	};
}