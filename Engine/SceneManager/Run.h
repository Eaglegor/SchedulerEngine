#pragma once

#include <Engine/Concepts/Site.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "SceneObjectsFactory.h"
#include "Operation.h"
#include "RunBoundaryStop.h"
#include <Engine/Utils/Collections/LinkedPointersList.h>
#include <Engine/Utils/Collections/LinkedPointersSublist.h>
#include <Engine/Utils/Collections/RangeTypeAdapter.h>
#include <Engine/SceneManager/WorkStop.h>
#include "Schedule.h"
#include "DurationActualizer.h"
#include <memory>
#include <Engine/Utils/AutoCastRange.h>

#include <SceneManager_export.h>
#include <boost/range/adaptor/transformed.hpp>
#include <Engine/Utils/DerivedTypeCaster.h>
#include <Engine/Utils/Optional.h>

namespace Scheduler
{
	class Schedule;
	class Vehicle;
	class RoutingService;
	class RoutingProfile;
	class ScheduleActualizationModel;
	class ScheduleValidationModel;
	class StructuralChangesObserver;

	class SCENEMANAGER_EXPORT Run
	{
	private:
		using StopsRange = Range<Schedule::StopsList>;
		using WorkStopsRange = Range<StopsRange>;
		
	public:
		using StopsList = StopsRange;
		using StopIterator = StopsList::iterator;
		using ConstStopIterator = StopsList::const_iterator;
		
		using WorkStopsList = RangeTypeAdapter<WorkStopsRange, WorkStop>;
		using WorkStopIterator = WorkStopsList::iterator;
		using ConstWorkStopIterator = WorkStopsList::const_iterator;
		
		struct Context
		{
			SceneObjectsFactory<WorkStop>& stops_factory;
			StructuralChangesObserver& structural_changes_observer;
			ArrivalTimeActualizer& arrival_time_actualizer;
		};
		
		Run(std::size_t id, const Context& context, Schedule& schedule, const Location& start_location, const Location& end_location, Schedule::StopsList &stops_list, Schedule::StopsList::const_iterator pos);
		~Run();

		std::size_t getId() const;

		const Schedule& getSchedule() const;
		Schedule& getSchedule();

		Optional<const Vehicle&> getVehicle() const;
		void setVehicle(Optional<const Vehicle&> vehicle);

		const RunBoundaryStop& getStartStop() const;
		RunBoundaryStop& getStartStop();

		const WorkStopsList& getWorkStops() const;
		WorkStopsList& getWorkStops();
		
		const StopsList& getStops() const;
		StopsList& getStops();

		const RunBoundaryStop& getEndStop() const;
		RunBoundaryStop& getEndStop();

		void allocateStartOperation(const Operation &operation);
		WorkStopIterator createWorkStop(ConstWorkStopIterator pos, const Operation &operation);
		void allocateEndOperation(const Operation &operation);

		void unallocateStartOperation(const Operation &operation);
		WorkStopIterator destroyWorkStop(ConstWorkStopIterator pos);
		void unallocateEndOperation(const Operation &operation);
		
		void swapWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator second);
		void reverseWorkStops();
		void reverseWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator last);
		void spliceWorkStops(ConstWorkStopIterator pos, ConstWorkStopIterator first, ConstWorkStopIterator last);
		void spliceWorkStops(ConstWorkStopIterator pos, Run& from, ConstWorkStopIterator first, ConstWorkStopIterator last, Optional<std::size_t> n = None);

		bool isValid() const;

		bool operator==(const Run& rhs) const;
		bool operator!=(const Run& rhs) const;
		
		StopIterator findStop(Stop& stop);
		ConstStopIterator findStop(const Stop& stop) const;
		
		WorkStopIterator findWorkStop(WorkStop& stop);
		ConstWorkStopIterator findWorkStop(const WorkStop& stop) const;
		
	private:
		bool isDetached() const;
		void detach();
		void attach(Schedule::StopsList::const_iterator pos);
		
		bool isConsistent() const;
		void setStopsEndIterator(ConstStopIterator end);

		std::size_t id;
		Context context;
		
		Schedule& schedule;
		Optional<const Vehicle&> vehicle;

		RunBoundaryStop start_stop;
		RunBoundaryStop end_stop;

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