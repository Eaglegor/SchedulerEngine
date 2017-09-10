#pragma once

#include "ArrivalTimeActualizer.h"
#include "Constraints/Schedule/ScheduleConstraints.h"
#include "Performer.h"
#include "SceneObjectsFactory.h"
#include "Stop.h"
#include <Engine/Concepts/Site.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Collections/LinkedPointersList.h>
#include <Engine/Utils/String.h>
#include <functional>
#include <memory>
#include <string>

#include <Engine/Utils/Collections/Range.h>
#include <boost/intrusive/list.hpp>

#include "ScheduleActualizationModel.h"
#include "ScheduleValidationModel.h"


namespace Scheduler
{
	class Scene;
	class Run;
	class Stop;
	class WorkStop;
	class RoutingService;
	class LoggingService;
	class RunVehicleBinder;
	class ScheduleValidationModel;
	class Location;
	class StructuralChangesObserver;
	class ViolationsConsumer;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief The work shift plan of the resource
	 * 
	 * @details Schedule represents the working day plan of the resource, i.e. the sequence of 
	 * orders that are assigned to the resource along with their corresponding planned arrival times and
	 * inter-location route information. Schedule consists of zero, one or multiple runs.
	 * 
	 * @sa Run
	 */
	class Schedule
	{
	private:
		using StopsIntrusiveList = boost::intrusive::list<Stop, boost::intrusive::value_traits<Stop::IntrusiveValueTraits>>;

	public:
		/**
		 * @brief List of runs in the schedule
		 */
		using RunsList         = std::vector<ReferenceWrapper<Run>>;

		/**
		 * @brief Iterator to the run of the schedule
		 */
		using RunIterator      = RunsList::iterator;

		/**
		 * @brief Iterator to the run of the schedule
		 */
		using ConstRunIterator = RunsList::const_iterator;

		/**
		 * @brief Interface of destruction functor for the run
		 * 
		 * @details To improve performance detaching runs can be used in some cases.
		 * Destruction procedure doesn't immediately destroy the run but just removes it from
		 * the runs list and transfers the ownership to the external code. Since the memory management
		 * is performed by the nested factory, external code has to use this factory to correctly
		 * destroy the object. To not expose the internals of memory management, this abstract interface
		 * is provided to the external code to make it possible to correctly destroy the run.
		 */
		using RunDestructor    = std::function<void(Run*)>;

		/**
		 * @brief List of stops in the schedule
		 */
		using StopsList = StopsIntrusiveList;

		/**
		 * @brief Internal context of the schedule
		 */
		struct Context
		{
			SceneObjectsFactory<Run>& runs_factory;
			SceneObjectsFactory<WorkStop>& stops_factory;
			StructuralChangesObserver& structural_changes_observer;
		};

		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of the schedule
		 * @param context Internal context of the schedule
		 * @param performer Performer which the schedule is defined for
		 * @param scene The scene which the schedule belongs to
		 */
		Schedule(std::size_t id, const Context& context, const Performer& performer, const Scene& scene);

		/**
		 * @brief Destructor
		 * 
		 * @details Destoys all runs
		 */
		~Schedule( );

		/**
		 * @brief Returns identifier of the schedule
		 * 
		 * @return Identifier of the schedule
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns name of the schedule
		 * 
		 * @return Name of the schedule
		 */
		const String& getName( ) const;

		/**
		 * @brief Sets name of the schedule
		 * 
		 * @param name Name of the schedule
		 */
		void setName(const String& name);

		/**
		 * @brief Returns performer which the schedule is defined for
		 * 
		 * @return Performer which the schedule is defined for
		 */
		const Performer& getPerformer( ) const;

		/**
		 * @brief Returns the list of runs in the schedule
		 * 
		 * @return List of runs in the schedule
		 */
		const RunsList& getRuns( ) const;

		/**
		 * @brief Creates new run
		 * 
		 * @param pos Iterator to the run to place the new run before
		 * @param from The start location of the run (e.g. depot location)
		 * @param to The end location of the run (e.g. depot location)
		 * 
		 * @sa Schedule::destroyRun
		 * 
		 * @return Iterator to the created run
		 */
		RunIterator createRun(ConstRunIterator pos, const Location& from, const Location& to);

		/**
		 * @brief Returns list of the stops in the schedule
		 * 
		 * @return List of the stops in the schedule
		 */
		const StopsList& getStops( ) const;

		/**
		 * @brief Returns list of the stops in the schedule
		 * 
		 * @todo Do we need this method?
		 * 
		 * @return List of the stops in the schedule
		 */
		StopsList& getStops( );

		/**
		 * @brief Checks if the schedule is empty
		 * 
		 * @return @c true, if the schedule contains no runs, @c false otherwise
		 */
		bool empty( ) const;

		/**
		 * @brief Destroys the run
		 * 
		 * @param pos Iterator to the run to destroy
		 * 
		 * @sa Schedule::createRun Schedule::detachRun
		 * 
		 * @return Iterator to the run after the destroyed one
		 */
		RunIterator destroyRun(ConstRunIterator pos);

		/**
		 * @brief Detaches the run
		 * 
		 * @details If there is a probability that the run will have to be restored after destruction
		 * it's a good idea to avoid reallocations of memory on destruction and creation procedures.
		 * One good way is to spare the run object for later and detach it from the schedule as if it was
		 * actually destroyed and then either destroy the run completely (performing memory deallocation)
		 * or just attach it back to the schedule as if it wasn't removed at all.
		 * Detached run has to be managed by the caller code. It can either be destroyed using the returned
		 * destruction functor or attached back (then the ownership is transferred back) to the schedule.
		 * 
		 * @warning No operations are allowed on detached runs
		 * 
		 * @param pos Iterator to the run to detach
		 * 
		 * @sa Schedule::attachRun
		 * 
		 * @return The pair of iterator pointing to the run after the detached one and the run destruction functor
		 */
		std::pair<RunIterator, RunDestructor> detachRun(ConstRunIterator pos);

		/**
		 * @brief Attaches the previously detached run
		 * 
		 * @param pos Iterator to the element to insert the run before
		 * @param run Detached run to attach to the schedule
		 * 
		 * @warning Only the run detached from this schedule has to be attached.
		 * Trying to attach the run from another schedule or the attached run may work but 
		 * has memory management issues (e.g. the factories for different schedules may 
		 * have different implementations). Doing so causes undefined behavior.
		 * 
		 * @sa Schedule::detachRun
		 * 
		 * @return Iterator to the attached run
		 */
		RunIterator attachRun(ConstRunIterator pos, Run& run);

		/**
		 * @brief Checks if schedule is valid in terms of hard constraints satisfaction
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
		 * @brief Sets actualization model for the schedule
		 * 
		 * @param model Actualization model for the schedule
		 * 
		 * @sa ScheduleActualizationModel
		 */
		void setActualizationModel(const ScheduleActualizationModel& model);

		/**
		 * @brief Sets validation model for the schedule
		 * 
		 * @param model Validation model for the schedule
		 * 
		 * @sa ScheduleValidationModel
		 */
		void setValidationModel(const ScheduleValidationModel& model);

		/**
		 * @brief Returns the schedule actualization model
		 * 
		 * @return Schedule actualization model
		 * 
		 * @sa ScheduleActualizationModel
		 */
		const ScheduleActualizationModel& getActualizationModel( ) const;

		/**
		 * @brief Returns the schedule validation model
		 * 
		 * @return Schedule validation model
		 * 
		 * @sa ScheduleValidationModel
		 */
		const ScheduleValidationModel& getValidationModel( ) const;

		/**
		 * @brief Returns the work shift time window of the schedule
		 * 
		 * @todo Rename this to AvailabilityTimeWindow?
		 * 
		 * @return Work shift time window of the scheduel
		 */
		const TimeWindow& getShift( ) const;

		/**
		 * @brief Sets the work shift time window of the schedule
		 * 
		 * @todo Rename this to AvailabilityTimeWindow?
		 * 
		 * @param shift Work shift time window of the scheduel
		 */
		void setShift(const TimeWindow& shift);

		/**
		 * @brief Removes all runs from the schedule
		 */
		void clear( );

		/**
		 * @brief Returns the scene this schedule belongs to
		 * 
		 * @return The scene this schedule belongs to
		 */
		const Scene& getScene( ) const;

		/**
		 * @brief Returns schedule constraints
		 * 
		 * @return Reference to the schedule constraints
		 */
		const ScheduleConstraints& constraints( ) const;

		/**
		 * @brief Returns schedule constraints
		 * 
		 * @return Reference to the schedule constraints
		 */
		ScheduleConstraints& constraints( );

		/**
		 * @brief Sets optional vehicle binder for this schedule
		 * 
		 * @details If the vehicle binder is set for the schedule the vehicles will automatically be
		 * assigned to the created runs. Run can't be performed without the vehicle assigned so to
		 * allow the algorithmic code not to manage these assignments explicitly the vehicle binder
		 * can be assigned to the schedule
		 * 
		 * @todo Should we remove this concept since complex vehicle assignments better have to be
		 * managed explicitly?
		 * 
		 * @param run_vehicle_binder Vehicle binder for the schedule
		 */
		void setRunVehicleBinder(Optional<const RunVehicleBinder&> run_vehicle_binder);

		/**
		 * @brief Returns vehicle binder of this schedule
		 * 
		 * @todo Should we remove this concept since complex vehicle assignments better have to be
		 * managed explicitly?
		 * 
		 * @sa Schedule::setRunVehicleBinder
		 * 
		 * @param Vehicle binder of the schedule
		 */
		Optional<const RunVehicleBinder&> getRunVehicleBinder( ) const;

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another schedule
		 * 
		 * @return @c true, if @c rhs is the same object as @c this, @c false otherwise
		 */
		bool operator==(const Schedule& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another schedule
		 * 
		 * @return Result of !(rhs == *this)
		 */
		bool operator!=(const Schedule& rhs) const;

		/**
		 * @brief Returns the iterator to the specified run
		 * 
		 * @param r Run to get the iterator to
		 * 
		 * @return Iterator to @c r or getRuns()::end() if there's no such run
		 */
		RunIterator findRun(Run& r);

		/**
		 * @brief Returns the iterator to the specified run
		 * 
		 * @param r Run to get the iterator to
		 * 
		 * @return Iterator to @c r or getRuns()::end() if there's no such run
		 */
		ConstRunIterator findRun(const Run& r) const;

	private:
		std::size_t id;
		Context context;
		const Performer& performer;
		const Scene& scene;

		String name;

		TimeWindow shift;

		ArrivalTimeActualizer arrival_time_actualizer;

		ScheduleActualizationModel schedule_actualization_model;
		ScheduleValidationModel schedule_validation_model;

		Optional<const RunVehicleBinder&> run_vehicle_binder;

		ScheduleConstraints schedule_constraints;

		RunsList runs;
		StopsList stops;
	};
}
