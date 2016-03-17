#pragma once

#include <cmath>
#include "Speed.h"
#include "Distance.h"
#include "Duration.h"

#include <Concepts_export.h>

namespace Scheduler
{
	CONCEPTS_EXPORT
	Distance operator*(const Speed &speed, const Duration &duration);

	CONCEPTS_EXPORT
	Distance operator*(const Duration &duration, const Speed &speed);

	CONCEPTS_EXPORT
	Duration operator/(const Distance &distance, const Speed &speed);

	CONCEPTS_EXPORT
	Speed operator/(const Distance &distance, const Duration &duration);
}