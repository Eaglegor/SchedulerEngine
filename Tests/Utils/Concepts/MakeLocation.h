#pragma once

#include <Engine/Concepts/Basic/Location.h>

namespace Scheduler
{
	Location make_location(float a, float b)
	{
		return Location(Coordinate(a), Coordinate(b));
	}
}