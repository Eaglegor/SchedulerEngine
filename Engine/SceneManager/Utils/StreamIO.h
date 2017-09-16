#pragma once

#include "../Depot.h"
#include "../Vehicle.h"

#include <boost/optional/optional_io.hpp>
#include <ostream>

namespace Scheduler
{
	/**
	 * @brief Operator used to output out the vehicle information to the ouput stream
	 * 
	 * @param stream Output stream to print the information to
	 * @param vehicle Vehicle for which the information will be printed
	 */
	std::ostream& operator<<(std::ostream& stream, const Vehicle& vehicle);

	/**
	 * @brief Operator used to output out the depot information to the ouput stream
	 * 
	 * @param stream Output stream to print the information to
	 * @param depot Depot for which the information will be printed
	 */
	std::ostream& operator<<(std::ostream& stream, const Depot& depot);
}