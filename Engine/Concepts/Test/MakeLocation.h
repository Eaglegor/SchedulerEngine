#pragma once

#include <Engine/Concepts/Location.h>

namespace Scheduler
{
	static Location make_location(float a, float b)
	{
		return Location(Coordinate::createFromFloat(a), Coordinate::createFromFloat(b));
	}
}