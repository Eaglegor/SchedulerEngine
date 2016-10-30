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
		return work_stops.begin();
	}
	
	InvariantWorkStopsList::const_iterator InvariantWorkStopsList::end() const
	{
		return work_stops.end();
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
		if(iter == work_stops.end()) return r.getWorkStops().end();
		return r.findWorkStop(*iter);
	}
};