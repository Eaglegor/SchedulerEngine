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

	class Run
	{
	private:
		using StopsRange     = Range<Schedule::StopsList>;
		using WorkStopsRange = Range<StopsRange>;

	public:
		using StopsList         = StopsRange;
		using StopIterator      = StopsList::iterator;
		using ConstStopIterator = StopsList::const_iterator;

		using WorkStopsList         = RangeTypeAdapter<WorkStopsRange, WorkStop>;
		using WorkStopIterator      = WorkStopsList::iterator;
		using ConstWorkStopIterator = WorkStopsList::const_iterator;

		struct Context
		{
			SceneObjectsFactory<WorkStop>& stops_factory;
			StructuralChangesObserver& structural_changes_observer;
			ArrivalTimeActualizer& arrival_time_actualizer;
		};

		Run(std::size_t id, const Context& context, Schedule& schedule, const Location& start_location, const Location& end_location, Schedule::StopsList& stops_list, Schedule::StopsList::const_iterator pos);
		~Run( );

		std::size_t getId( ) const;

		const Schedule& getSchedule( ) const;
		Schedule& getSchedule( );

		Optional<const Vehicle&> getVehicle( ) const;
		void setVehicle(Optional<const Vehicle&> vehicle);

		const DepotStop& getStartStop( ) const;
		DepotStop& getStartStop( );

		const WorkStopsList& getWorkStops( ) const;
		WorkStopsList& getWorkStops( );

		const StopsList& getStops( ) const;
		StopsList& getStops( );

		const DepotStop& getEndStop( ) const;
		DepotStop& getEndStop( );

		WorkStopIterator allocateOrder(ConstWorkStopIterator pos, const Order& order);
		WorkStopIterator unallocateOrder(ConstWorkStopIterator pos);

		void swapWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator second);
		void reverseWorkStops( );
		void reverseWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator last);
		void spliceWorkStops(ConstWorkStopIterator pos, ConstWorkStopIterator first, ConstWorkStopIterator last);
		void spliceWorkStops(ConstWorkStopIterator pos, Run& from, ConstWorkStopIterator first, ConstWorkStopIterator last, Optional<std::size_t> n = None);

		bool isValid( ) const;
		void validate(ViolationsConsumer& violations_consumer) const;

		bool operator==(const Run& rhs) const;
		bool operator!=(const Run& rhs) const;

		StopIterator findStop(Stop& stop);
		ConstStopIterator findStop(const Stop& stop) const;

		WorkStopIterator findWorkStop(WorkStop& stop);
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
