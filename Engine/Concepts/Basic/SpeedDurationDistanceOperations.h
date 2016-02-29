#pragma once

#include <cmath>
#include "Speed.h"
#include "Distance.h"
#include "Duration.h"

namespace Scheduler
{
	Distance operator*(const Speed &speed, const Duration &duration);

	Distance operator*(const Duration &duration, const Speed &speed);

	Duration operator/(const Distance &distance, const Speed &speed);

	Speed operator/(const Distance &distance, const Duration &duration);
}