#pragma once

#include "../Schedule.h"
#include "../Run.h"

namespace Scheduler
{
	template<typename T>
	typename T::iterator removeIteratorConstness(typename T::const_iterator iter)
	{
		return iter;
	};
	
	template<>
	Schedule::StopsList::iterator removeIteratorConstness<Schedule::StopsList>(Schedule::StopsList::const_iterator iter)
	{
		return iter.unconst();
	};
	
	template<>
	Run::StopsList::iterator removeIteratorConstness<Run::StopsList>(Run::StopsList::const_iterator iter)
	{
		return iter.unconst();
	};
	
	template<>
	Run::WorkStopsList::iterator removeIteratorConstness<Run::WorkStopsList>(Run::WorkStopsList::const_iterator iter)
	{
		return Run::WorkStopsList::iterator(iter.base().unconst());
	};	
	
}