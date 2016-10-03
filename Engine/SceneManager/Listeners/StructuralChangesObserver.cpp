#include "StructuralChangesObserver.h"

namespace Scheduler
{
	void StructuralChangesObserver::addListener(StructuralChangesListener* listener)
	{
		listeners.emplace(listener);
	}
	
	void StructuralChangesObserver::removeListener(StructuralChangesListener* listener)
	{
		listeners.erase(listener);
	}
	
	void StructuralChangesObserver::afterStartOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->afterStartOperationAdded(iter, operation);
		}
	}
	
	void StructuralChangesObserver::beforeStartOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeStartOperationRemoved(iter, operation);
		}
	}
	
	void StructuralChangesObserver::afterEndOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->afterEndOperationAdded(iter, operation);
		}
	}
	
	void StructuralChangesObserver::beforeEndOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeEndOperationRemoved(iter, operation);
		}
	}
	
	void StructuralChangesObserver::afterRunCreated(Schedule::RunsList::const_iterator iter)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->afterRunCreated(iter);
		}
	}
	
	void StructuralChangesObserver::beforeRunDestroyed(Schedule::RunsList::const_iterator iter)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeRunDestroyed(iter);
		}
	}
	
	void StructuralChangesObserver::afterWorkStopCreated(Run::WorkStopsList::const_iterator iter)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->afterWorkStopCreated(iter);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopDestroyed(Run::WorkStopsList::const_iterator iter)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeWorkStopDestroyed(iter);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopsReversed(Run::WorkStopsList::const_iterator range_begin, Run::WorkStopsList::const_iterator range_end)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeWorkStopsReversed(range_begin, range_end);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopsSwapped(Run::WorkStopsList::const_iterator first, Run::WorkStopsList::const_iterator second)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeWorkStopsSwapped(first, second);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopsSpliced(Run::WorkStopsList::const_iterator pos, Run::WorkStopsList::const_iterator range_begin, Run::WorkStopsList::const_iterator range_end)
	{
		for(StructuralChangesListener* listener : listeners)
		{
			listener->beforeWorkStopsSpliced(pos, range_begin, range_end);
		}
	}

}