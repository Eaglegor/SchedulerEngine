#pragma once

#include <Engine/Concepts/Basic/Distance.h>
#include <Engine/Concepts/Basic/Speed.h>

namespace Scheduler
{
	namespace Units
	{
		Speed speedPerHour(const Distance &distance)
		{
			return Speed(Distance(distance) / 3600.0f);
		}

		Speed speedPerHour(float distance)
		{
			return Speed(Distance(distance) / 3600.0f);
		}
	}
}