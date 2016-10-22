#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Run;
	
	class RunValidationAlgorithm : public Algorithm
	{
		public:
			virtual bool isValid(const Run& run) const = 0;
	};
}