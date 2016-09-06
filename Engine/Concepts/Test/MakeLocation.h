#pragma once

#include <Engine/Concepts/Site.h>

namespace Scheduler
{
	static Site make_location(float a, float b)
	{
		return Site(Coordinate::createFromFloat(a), Coordinate::createFromFloat(b));
	}
}