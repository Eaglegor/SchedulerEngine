#include "InvariantWorkStopsList.h"

namespace Scheduler
{
	InvariantWorkStopsList::InvariantWorkStopsList(Run::WorkStopsList& stops)
	{
		for(WorkStop& stop : stops)
		{
			work_stops.emplace_back(stop);
		}
	}
	
	InvariantWorkStopsList::const_iterator InvariantWorkStopsList::begin() const
	{
		return const_iterator(&work_stops, 0);
	}
	
	InvariantWorkStopsList::const_iterator InvariantWorkStopsList::end() const
	{
		return const_iterator(&work_stops, work_stops.size());
	}
	
	InvariantWorkStopsList& InvariantWorkStopsList::operator=(InvariantWorkStopsList&& rhs)
	{
		work_stops = std::move(rhs.work_stops);
		return *this;
	}
	
	InvariantWorkStopsList& InvariantWorkStopsList::operator=(const Run::WorkStopsList& rhs)
	{
		work_stops.clear();
		for(const WorkStop& stop : rhs)
		{
			work_stops.emplace_back(stop);
		}
		return *this;
	}

	Run::ConstWorkStopIterator InvariantWorkStopsList::getRunWorkStopIterator(const Run& r, const_iterator iter)
	{
		if(iter == end()) return r.getWorkStops().end();
		return r.findWorkStop(*iter);
	}
};