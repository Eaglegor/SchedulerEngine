#include "StructuralChangesObserver.h"

namespace Scheduler
{
	void StructuralChangesObserver::addListener(StructuralChangesListener& listener)
	{
		listeners.emplace(listener);
	}
	
	void StructuralChangesObserver::removeListener(StructuralChangesListener& listener)
	{
		listeners.erase(listener);
	}
	
	void StructuralChangesObserver::afterStartOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.afterStartOperationAdded(iter, operation);
		}
	}
	
	void StructuralChangesObserver::beforeStartOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.beforeStartOperationRemoved(iter, operation);
		}
	}
	
	void StructuralChangesObserver::afterEndOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.afterEndOperationAdded(iter, operation);
		}
	}
	
	void StructuralChangesObserver::beforeEndOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.beforeEndOperationRemoved(iter, operation);
		}
	}
	
	void StructuralChangesObserver::afterRunCreated(Schedule::ConstRunIterator iter)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.afterRunCreated(iter);
		}
	}
	
	void StructuralChangesObserver::beforeRunDestroyed(Schedule::ConstRunIterator iter)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.beforeRunDestroyed(iter);
		}
	}
	
	void StructuralChangesObserver::afterWorkStopCreated(Run::ConstWorkStopIterator iter)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.afterWorkStopCreated(iter);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopDestroyed(Run::ConstWorkStopIterator iter)
	{
		for(StructuralChangesListener&listener : listeners)
		{
			listener.beforeWorkStopDestroyed(iter);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopsReversed(Run::ConstWorkStopIterator range_begin, Run::ConstWorkStopIterator range_end)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.beforeWorkStopsReversed(range_begin, range_end);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopsSwapped(Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.beforeWorkStopsSwapped(first, second);
		}
	}
	
	void StructuralChangesObserver::beforeWorkStopsSpliced(const Run& to_run, Run::ConstWorkStopIterator pos, const Run& from_run, Run::ConstWorkStopIterator range_begin, Run::ConstWorkStopIterator range_end)
	{
		for(StructuralChangesListener& listener : listeners)
		{
			listener.beforeWorkStopsSpliced(to_run, pos, from_run, range_begin, range_end);
		}
	}

}
