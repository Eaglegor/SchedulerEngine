#pragma once

namespace Scheduler
{
	class Run;
	
	class RunValidationAlgorithm
	{
		public:
			virtual ~RunValidationAlgorithm(){}

			virtual bool isValid(const Run* run) const = 0;
	};
}