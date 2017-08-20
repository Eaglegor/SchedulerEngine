#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/Concepts/Speed.h>

namespace Scheduler
{
	namespace Units
	{
		static Speed speedPerHour(const Distance& distance)
		{
			return Speed(Distance(distance) / 3600.0f);
		}

		static Speed speedPerHour(float distance)
		{
			return Speed(Distance(distance) / 3600.0f);
		}
	}
}