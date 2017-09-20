#pragma once

#include "DepotStop.h"
#include "DurationActualizer.h"
#include "Operation.h"
#include "SceneObjectsFactory.h"
#include "Schedule.h"
#include <Engine/Concepts/Site.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/AutoCastRange.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Collections/LinkedPointersList.h>
#include <Engine/Utils/Collections/LinkedPointersSublist.h>
#include <Engine/Utils/Collections/RangeTypeAdapter.h>
#include <memory>

#include <Engine/Utils/DerivedTypeCaster.h>
#include <Engine/Utils/Optional.h>

#include <boost/range/adaptor/transformed.hpp>

namespace Scheduler
{
	class Schedule;
	class Vehicle;
	class RoutingService;
	class RoutingProfile;
	class ScheduleActualizationModel;
	class ScheduleValidationModel;
	class StructuralChangesObserver;
	class ViolationsConsumer;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Part of resource schedule
	 * 
	 * @details Run is the part of resource schedule consisting of:
	 *   -# Performing zero or more start operations in the depot (at the start stop)
	 *   -# Leaving the depot
	 *   -# Performing one or more work operations at customers locations
	 *   -# Returning to the depot
	 *   -# Performing zero or more end operations in the depot (at the end stop)
	 * 
	 * Single resource can have multiple runs per schedule, e.g. if it's capacity isn't enough to deliver
	 * all orders in a single run. Different runs inside the single schedule may use different vehicles if
	 * there is such business requirement and the selected algorithms support such vehicles assignment.
	 * 
	 * @sa @ref scene_structure
	 *
	 */
	class Run
	{
	private:
		using StopsRange     = Range<Schedule::StopsList>;
		using WorkStopsRange = Range<StopsRange>;

	public:
		/**
		 * @brief List of stops in the run
		 * 
		 * @details Run stops list is a subsequence of schedule stops list
		 * In fact only the schedule stores the list of stops. Runs just point to
		 * the range inside this list.
		 */
		using StopsList         = StopsRange;

		/**
		 * @brief Iterator to the stop in the run
		 */
		using StopIterator      = StopsList::iterator;

		/**
		 * @brief Iterator to the stop in the run
		 */
		using ConstStopIterator = StopsList::const_iterator;

		/**
		 * @brief List of work stops in the run
		 * 
		 * @details Like run stops list is a range inside the schedule stops list,
		 * run work stops list is a range inside run stops list. Being nested Range
		 * type it points to the range inside the schedule stops list and implicitly
		 * casts abstract Stop type to the WorkStop.
		 */
		using WorkStopsList         = RangeTypeAdapter<WorkStopsRange, WorkStop>;

		/**
		 * @brief Iterator to the work stop in the run
		 */
		using WorkStopIterator      = WorkStopsList::iterator;

		/**
		 * @brief Iterator to the work stop in the run
		 */
		using ConstWorkStopIterator = WorkStopsList::const_iterator;

		/**
		 * @brief Internal context of the run
		 */
		struct Context
		{
			/**
			 * @brief Factory used to create work stops
			 */
			SceneObjectsFactory<WorkStop>& stops_factory;

			/**
			 * @brief Observer to notify about run structural changes
			 */
			StructuralChangesObserver& structural_changes_observer;

			/**
			 * @brief Arrival time actualizer associated with the run's parent schedule
			 */
			ArrivalTimeActualizer& arrival_time_actualizer;
		};

		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of run
		 * @param context Internal context of run
		 * @param start_location Location where the run starts (where start operations are performed, e.g. depot)
		 * @param end_location End location of the run (where end operations are performed, e.g. depot)
		 * @param stops_list Reference to the parent schedule's stops list
		 * @param pos Iterator to the stop in the parent schedule's stops list before which the run stops will be inserted
		 */
		Run(std::size_t id, const Context& context, Schedule& schedule, const Location& start_location, const Location& end_location, Schedule::StopsList& stops_list, Schedule::StopsList::const_iterator pos);

		/**
		 * @brief Destructor
		 * 
		 * @details Destroys all work stops using the provided factory
		 */
		~Run( );

		/**
		 * @brief Returns the identifier of run
		 * 
		 * @return Identifier of run
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns the parent schedule
		 * 
		 * @return Parent schedule
		 */
		const Schedule& getSchedule( ) const;

		/**
		 * @brief Returns the parent schedule
		 * 
		 * @return Parent schedule
		 */
		Schedule& getSchedule( );

		/**
		 * @brief Returns the vehicle assigned to the run
		 * 
		 * @return Vehicle assigned to the run
		 */
		Optional<const Vehicle&> getVehicle( ) const;

		/**
		 * @brief Assigns the vehicle to the run
		 * 
		 * @param vehicle Vehicle to assign to the run
		 */
		void setVehicle(Optional<const Vehicle&> vehicle);

		/**
		 * @brief Returns start stop of the run (e.g. depot or performer's home)
		 * 
		 * @return Start stop of the run
		 */
		const DepotStop& getStartStop( ) const;

		/**
		 * @brief Returns start stop of the run (e.g. depot or performer's home)
		 * 
		 * @return Start stop of the run
		 */
		DepotStop& getStartStop( );

		/**
		 * @brief Returns work stops of the run
		 * 
		 * @note The returned list doesn't include start and end stops of the run
		 * 
		 * @return List of work stops of the run
		 */
		const WorkStopsList& getWorkStops( ) const;

		/**
		 * @brief Returns work stops of the run
		 * 
		 * @note The returned list doesn't include start and end stops of the run
		 * 
		 * @return List of work stops of the run
		 */
		WorkStopsList& getWorkStops( );

		/**
		 * @brief Returns all stops of the run
		 * 
		 * @return List of all stops of the run (including start and end stop)
		 */
		const StopsList& getStops( ) const;

		/**
		 * @brief Returns all stops of the run
		 * 
		 * @return List of all stops of the run (including start and end stop)
		 */
		StopsList& getStops( );

		/**
		 * @brief Returns end stop of the run (e.g. depot or performer's home)
		 * 
		 * @return End stop of the run
		 */
		const DepotStop& getEndStop( ) const;

		/**
		 * @brief Returns end stop of the run (e.g. depot or performer's home)
		 * 
		 * @return End stop of the run
		 */
		DepotStop& getEndStop( );

		/**
		 * @brief Allocates order into the specified position of the run
		 * 
		 * @details The following steps take place in this method:
		 *   -# If order contains start operation, this operation is added to the run's start stop.
		 *   -# Work stop is created and associated with work operation of order
		 *   -# If order contains end operation, this operation is added to the run's end stop.
		 * 
		 * @param pos Iterator to the work stop before which the order will be allocated
		 * @param order Order to allocate into the run
		 * 
		 * @return Iterator to the created work stop
		 */
		WorkStopIterator allocateOrder(ConstWorkStopIterator pos, const Order& order);

		/**
		 * @brief Unallocates order associated with the work stop at the specified position
		 * 
		 * @details The following steps take place in this method:
		 *   -# If order contains start operation, this operation is removed from the run's start stop.
		 *   -# Work stop is detached from the run and destroyed
		 *   -# If order contains end operation, this operation is removed from the run's end stop.
		 * 
		 * @param pos Iterator to the work stop associated with the order to unallocate
		 * 
		 * @return Iterator to the work stop after the destroyed one
		 */
		WorkStopIterator unallocateOrder(ConstWorkStopIterator pos);

		/**
		 * @brief Swaps work stops pointed by first and second
		 * 
		 * @param first First work stop
		 * @param second Second work stop
		 */
		void swapWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator second);

		/**
		 * @brief Reverses the order of work stops in the run
		 */
		void reverseWorkStops( );

		/**
		 * @brief Reverses the order of work stops in a range
		 * 
		 * @param first Iterator to the first order in range to reverse
		 * @param last Iterator to the order after the last order in range to reverse
		 */
		void reverseWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator last);

		/**
		 * @brief Moves work stops range inside the run
		 * 
		 * @param pos Iterator to the work stop before which the range will be inserted
		 * @param first Iterator to the first order in range to move
		 * @param last Iterator to the order after the last order in range to move
		 */
		void spliceWorkStops(ConstWorkStopIterator pos, ConstWorkStopIterator first, ConstWorkStopIterator last);

		/**
		 * @brief Moves work stops from the specified run
		 * 
		 * @param pos Iterator to the work stop before which the range will be inserted
		 * @param from Run to transfer work stops from (may be the same run)
		 * @param first Iterator to the first order in range to move
		 * @param last Iterator to the order after the last order in range to move
		 */
		void spliceWorkStops(ConstWorkStopIterator pos, Run& from, ConstWorkStopIterator first, ConstWorkStopIterator last, Optional<std::size_t> n = None);

		/**
		 * @brief Checks if run is valid in terms of hard constraints satisfaction
		 * 
		 * @return true, if no hard constraints are violated
		 */
		bool isValid( ) const;

		/**
		 * @brief Reports hard constraints violations to the specified consumer
		 * 
		 * @param violations_consumer Consumer that will process the reported violations
		 */
		void validate(ViolationsConsumer& violations_consumer) const;

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another run
		 * 
		 * @return true, if rhs is the same object as this, false otherwise
		 */
		bool operator==(const Run& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another run
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Run& rhs) const;

		/**
		 * @brief Returns iterator to the specified stop
		 * 
		 * @warning This method assumes intrusive nature of stops list (iterator can be retrieved in a constant-time from a pointer)
		 * It doesn't perform any linear operations and just wraps the passed pointer to the iterator bound to the run stops list.
		 * Passing stop from another run will produce the valid iterator but pointing to the another run's stop list.
		 * 
		 * @param stop Stop to get iterator for
		 * 
		 * @return Iterator pointing to the specified stop
		 */
		StopIterator findStop(Stop& stop);

		/**
		 * @brief Returns iterator to the specified stop
		 * 
		 * @warning This method assumes intrusive nature of stops list (iterator can be retrieved in a constant-time from a pointer)
		 * It doesn't perform any linear operations and just wraps the passed pointer to the iterator bound to the run stops list.
		 * Passing stop from another run will produce the valid iterator but pointing to the another run's stop list.
		 * 
		 * @param stop Stop to get iterator for
		 * 
		 * @return Iterator pointing to the specified stop
		 */
		ConstStopIterator findStop(const Stop& stop) const;

		/**
		 * @brief Returns iterator to the specified work stop
		 *
		 * @warning This method assumes intrusive nature of work stops list (iterator can be retrieved in a constant-time from a pointer)
		 * It doesn't perform any linear operations and just wraps the passed pointer to the iterator bound to the run work stops list.
		 * Passing stop from another run will produce the valid iterator but pointing to the another run's work stop list.
		 *  
		 * @param stop Work stop to get iterator for
		 * 
		 * @return Iterator pointing to the specified work stop
		 */
		WorkStopIterator findWorkStop(WorkStop& stop);

		/**
		 * @brief Returns iterator to the specified stop
		 * 
		 * @warning This method assumes intrusive nature of work stops list (iterator can be retrieved in a constant-time from a pointer)
		 * It doesn't perform any linear operations and just wraps the passed pointer to the iterator bound to the run work stops list.
		 * Passing stop from another run will produce the valid iterator but pointing to the another run's work stop list.
		 * 
		 * @param stop Work stop to get iterator for
		 * 
		 * @return Iterator pointing to the specified work stop
		 */
		ConstWorkStopIterator findWorkStop(const WorkStop& stop) const;

	private:
		void allocateStartOperation(const DepotOperation& operation);
		void allocateEndOperation(const DepotOperation& operation);

		void unallocateStartOperation(const DepotOperation& operation);
		void unallocateEndOperation(const DepotOperation& operation);

		bool isDetached( ) const;
		void detach( );
		void attach(Schedule::StopsList::const_iterator pos);

		bool isConsistent( ) const;
		void setStopsEndIterator(ConstStopIterator end);

		std::size_t id;
		Context context;

		Schedule& schedule;
		Optional<const Vehicle&> vehicle;

		DepotStop start_stop;
		DepotStop end_stop;

		DurationActualizer duration_actualizer;

		StopsRange stops;
		WorkStopsRange work_stops;
		WorkStopsList casted_work_stops;

		Schedule::StopsList& schedule_stops;
		Schedule::StopsList detached_stops;

		bool is_detached;

		friend class Schedule;
	};
}
