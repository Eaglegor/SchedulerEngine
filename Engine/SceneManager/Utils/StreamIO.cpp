#include "StreamIO.h"


namespace Scheduler 
{
	std::ostream& operator<<(std::ostream& stream, const Scheduler::Depot& depot)
	{
		return stream << &depot;
	}

	std::ostream& operator<<(std::ostream& stream, const Vehicle& vehicle)
	{
		return stream << &vehicle;
	}
}