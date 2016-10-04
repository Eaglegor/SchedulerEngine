#pragma once

#include <unordered_set>
#include "StructuralChangesListener.h"
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT StructuralChangesObserver : public StructuralChangesListener
	{
	public:
		void addListener(StructuralChangesListener* listener);
		void removeListener(StructuralChangesListener* listener);
		
		virtual void afterWorkStopCreated(Run::WorkStopsList::const_iterator iter);
		virtual void beforeWorkStopDestroyed(Run::WorkStopsList::const_iterator iter);
		
		virtual void afterRunCreated(Schedule::RunsList::const_iterator iter);
		virtual void beforeRunDestroyed(Schedule::RunsList::const_iterator iter);
		
		virtual void afterStartOperationAdded(Schedule::StopsList::const_iterator iter, const Operation* operation);
		virtual void beforeStartOperationRemoved(Schedule::StopsList::const_iterator iter, const Operation* operation);
		
		virtual void afterEndOperationAdded(Schedule::StopsList::const_iterator iter, const Operation* operation);
		virtual void beforeEndOperationRemoved(Schedule::StopsList::const_iterator iter, const Operation* operation);
		
		virtual void beforeWorkStopsSwapped(Run::WorkStopsList::const_iterator first, Run::WorkStopsList::const_iterator second);
		virtual void beforeWorkStopsReversed(Run::WorkStopsList::const_iterator range_begin, Run::WorkStopsList::const_iterator range_end);
		virtual void beforeWorkStopsSpliced(Run* to_run, Run::WorkStopsList::const_iterator pos, Run* from_run, Run::WorkStopsList::const_iterator range_begin, Run::WorkStopsList::const_iterator range_end);
		
	private:
		std::unordered_set<StructuralChangesListener*> listeners;
	};
}