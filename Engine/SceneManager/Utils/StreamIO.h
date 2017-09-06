#pragma once

#include "../Depot.h"
#include "../Vehicle.h"

#include <boost/optional/optional_io.hpp>
#include <ostream>

namespace Scheduler
{
	std::ostream& operator<<(std::ostream& stream, const Vehicle& vehicle);
	;
	std::ostream& operator<<(std::ostream& stream, const Depot& depot);
}