#pragma once

#include <string>
#include <Engine/Concepts/Site.h>
#include "Performer.h"
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "SceneObjectsFactory.h"
#include "Constraints/Schedule/ScheduleConstraints.h"
#include <Engine/Utils/Collections/LinkedPointersList.h>
#include <Engine/Utils/String.h>
#include "ArrivalTimeActualizer.h"
#include "Stop.h"
#include <memory>
#include <functional>

#include <boost/intrusive/list.hpp>
#include <Engine/Utils/Collections/Range.h>

#include <SceneManager_export.h>
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
		Class representing single performer's work shift.
	*/
    class SCENEMANAGER_EXPORT Schedule
    {
	private:
		using StopsIntrusiveList = boost::intrusive::list<Stop, boost::intrusive::value_traits<Stop::IntrusiveValueTraits>>;
		
    public:
		using RunsList = std::vector<ReferenceWrapper<Run>>;
		using RunIterator = RunsList::iterator;
		using ConstRunIterator = RunsList::const_iterator;
		using RunDestructor = std::function< void(Run*) >;
		
		using StopsList = StopsIntrusiveList;
		

		struct Context
		{
			SceneObjectsFactory<Run>& runs_factory;
			SceneObjectsFactory<WorkStop>& stops_factory;
			StructuralChangesObserver& structural_changes_observer;
		};
		
        Schedule(std::size_t id, const Context& context, const Performer &performer, const Scene& scene);
		~Schedule();

        std::size_t getId() const;
		
        const String& getName() const;
        void setName(const String& name);
        
		const Performer& getPerformer() const;

		const RunsList& getRuns() const;
		RunIterator createRun(ConstRunIterator pos, const Location &from, const Location &to);

		const StopsList& getStops() const;
		StopsList& getStops();
		
		bool empty() const;

		RunIterator destroyRun(ConstRunIterator pos);
		
		std::pair<RunIterator, RunDestructor> detachRun(ConstRunIterator pos);
		RunIterator attachRun(ConstRunIterator pos, Run& run);

        bool isValid() const;
		void validate(ViolationsConsumer& violations_consumer) const;

		void setActualizationModel(const ScheduleActualizationModel& model);
		void setValidationModel(const ScheduleValidationModel& model);
		
		const ScheduleActualizationModel& getActualizationModel() const;
		const ScheduleValidationModel& getValidationModel() const;

		const TimeWindow& getShift() const;
		void setShift(const TimeWindow &shift);

		void clear();

		const Scene& getScene() const;

		const ScheduleConstraints& constraints() const;
		ScheduleConstraints& constraints();

		void setRunVehicleBinder(Optional<const RunVehicleBinder&> run_vehicle_binder);
		Optional<const RunVehicleBinder&> getRunVehicleBinder() const;
		
		bool operator==(const Schedule& rhs) const;
		bool operator!=(const Schedule& rhs) const;
		
		RunIterator findRun(Run& r);
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
