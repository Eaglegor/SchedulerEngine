#pragma once

#include "../Run.h"
#include "../Schedule.h"
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT StructuralChangesListener
	{
	public: 
		virtual ~StructuralChangesListener(){}
		
		virtual void afterWorkStopCreated(Run::WorkStopsList::const_iterator iter){}
		virtual void beforeWorkStopDestroyed(Run::WorkStopsList::const_iterator iter){}
		
		virtual void afterRunCreated(Schedule::RunsList::const_iterator iter){}
		virtual void beforeRunDestroyed(Schedule::RunsList::const_iterator iter){}
		
		virtual void afterStartOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation){}
		virtual void beforeStartOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation){}
		
		virtual void afterEndOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation){}
		virtual void beforeEndOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation){}
		
		virtual void beforeWorkStopsSwapped(Run::WorkStopsList::const_iterator first, Run::WorkStopsList::const_iterator second){}
		virtual void beforeWorkStopsReversed(Run::WorkStopsList::const_iterator range_begin, Run::WorkStopsList::const_iterator range_end){}
		virtual void beforeWorkStopsSpliced(Run* to_run, Run::WorkStopsList::const_iterator pos, Run* from_run, Run::WorkStopsList::const_iterator range_begin, Run::WorkStopsList::const_iterator range_end){}
	};
}