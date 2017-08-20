#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
	class Vehicle;
	class Run;

	class RunVehicleBinder : public Algorithm
	{
	public:
		virtual bool bindVehicle(Run& run) const = 0;
	};
}