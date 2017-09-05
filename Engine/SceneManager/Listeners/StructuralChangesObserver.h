#pragma once

#include "StructuralChangesListener.h"
#include <Engine/Utils/ReferenceWrapper.h>

#include <unordered_set>

namespace Scheduler
{
	class StructuralChangesObserver : public StructuralChangesListener
	{
	public:
		void addListener(StructuralChangesListener& listener);
		void removeListener(StructuralChangesListener& listener);

		virtual void afterWorkStopCreated(Run::ConstWorkStopIterator iter);
		virtual void beforeWorkStopDestroyed(Run::ConstWorkStopIterator iter);

		virtual void afterRunCreated(Schedule::ConstRunIterator iter);
		virtual void beforeRunDestroyed(Schedule::ConstRunIterator iter);

		virtual void afterStartOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation);
		virtual void beforeStartOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation);

		virtual void afterEndOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation);
		virtual void beforeEndOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation);

		virtual void beforeWorkStopsSwapped(Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second);
		virtual void beforeWorkStopsReversed(Run::ConstWorkStopIterator range_begin, Run::ConstWorkStopIterator range_end);
		virtual void beforeWorkStopsSpliced(const Run& to_run, Run::ConstWorkStopIterator pos, const Run& from_run, Run::ConstWorkStopIterator range_begin, Run::ConstWorkStopIterator range_end);

	private:
		std::unordered_set<ReferenceWrapper<StructuralChangesListener>> listeners;
	};
}
