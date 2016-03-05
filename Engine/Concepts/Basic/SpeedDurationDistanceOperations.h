#pragma once

#include <cmath>
#include "Speed.h"
#include "Distance.h"
#include "Duration.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	SCHEDULERENGINE_EXPORT
	Distance operator*(const Speed &speed, const Duration &duration);

	SCHEDULERENGINE_EXPORT
	Distance operator*(const Duration &duration, const Speed &speed);

	SCHEDULERENGINE_EXPORT
	Duration operator/(const Distance &distance, const Speed &speed);

	SCHEDULERENGINE_EXPORT
	Speed operator/(const Distance &distance, const Duration &duration);
}