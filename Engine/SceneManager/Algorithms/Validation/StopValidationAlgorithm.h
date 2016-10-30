#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Stop;
	
	class StopValidationAlgorithm : public Algorithm
	{
		public:
			virtual bool isValid(const Stop& stop) const = 0;
	};
}