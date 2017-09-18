#pragma once

#include "StructuralChangesListener.h"
#include <Engine/Utils/ReferenceWrapper.h>

#include <unordered_set>

namespace Scheduler
{
	/**
	 * @brief Observer notifying subscribed listeners about the structural change in the scene
	 * 
	 * @sa @ref changes_listeners
	 */
	class StructuralChangesObserver : public StructuralChangesListener
	{
	public:
		/**
		 * @brief Subscribes the new structural changes listener
		 * 
		 * @param listener Listener to subscribe
		 */
		void addListener(StructuralChangesListener& listener);

		/**
		 * @brief Unsubscribes the new structural changes listener
		 * 
		 * @param listener Listener to unsubscribe
		 */
		void removeListener(StructuralChangesListener& listener);

		virtual void afterWorkStopCreated(Run::ConstWorkStopIterator iter);
		virtual void beforeWorkStopDestroyed(Run::ConstWorkStopIterator iter);

		/**
		 * @brief Is called after the new run was created in the schedule
		 * 
		 * @param iter Iterator to the created run
		 */
		virtual void afterRunCreated(Schedule::ConstRunIterator iter);

		/**
		 * @brief Is called before the run is destroyed
		 * 
		 * @param iter Iterator to the run that will be destroyed
		 */
		virtual void beforeRunDestroyed(Schedule::ConstRunIterator iter);

		virtual void afterStartOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation);
		virtual void beforeStartOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation);

		virtual void afterEndOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation);
		virtual void beforeEndOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation);

		/**
		 * @brief Is called before any actions in Run::swapWorkStops are performed
		 * 
		 * @param first The first stop to swap
		 * @param second The second stop to swap
		 */
		virtual void beforeWorkStopsSwapped(Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second);
		
		/**
		 * @brief Is called before any actions in Run::reverseWorkStops are performed
		 * 
		 * @param range_begin Iterator to the first stop in the range to be reversed
		 * @param range_end Iterator to the stop after the last stop in the range to be reversed
		 */
		virtual void beforeWorkStopsReversed(Run::ConstWorkStopIterator range_begin, Run::ConstWorkStopIterator range_end);
		
		/**
		 * @brief Is called before any actions in Run::spliceWorkStops are performed
		 * 
		 * @param to_run Run into which the transfer is to be done
		 * @param pos Position where the spliced stops will be inserted
		 * @param from_run Run from which the transfer is to be done
		 * @param range_start Iterator to the first stop in the range to be transferred
		 * @param range_end Iterator to the stop after the last stop in the range to be transferred
		 */
		virtual void beforeWorkStopsSpliced(const Run& to_run, Run::ConstWorkStopIterator pos, const Run& from_run, Run::ConstWorkStopIterator range_begin, Run::ConstWorkStopIterator range_end);

	private:
		std::unordered_set<ReferenceWrapper<StructuralChangesListener>> listeners;
	};
}
