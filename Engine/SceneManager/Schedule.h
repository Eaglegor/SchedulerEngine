#pragma once

#include <string>
#include <Engine/Concepts/Location.h>
#include "Performer.h"
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "SceneObjectsFactory.h"
#include "Constraints/Schedule/ScheduleConstraints.h"
#include <Engine/Utils/Collections/LinkedPointersList.h>

#include <SceneManager_export.h>
#include "ScheduleActualizationModel.h"

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

	/**
		Class representing single performer's work shift.
	*/
    class SCENEMANAGER_EXPORT Schedule
    {
    public:
		using RunsList = ImmutableVector<Run*>;
		using StopsList = LinkedPointersList<Stop*>;
		
        Schedule(size_t id, const Performer *performer);
		~Schedule();

        size_t getId() const;
        const char* getName() const;
        const Performer* getPerformer() const;

        void setName(const char* name);

		const RunsList& getRuns() const;

		RunsList::iterator createRun(RunsList::const_iterator pos, const Location &from, const Location &to);

		const StopsList& getStops() const;
		
		void destroyRun(RunsList::iterator pos);

		const Location& getDepotLocation() const;
		
		void setDepotLocation(const Location &depot_location);

        bool isValid() const;

		void setActualizationModel(ScheduleActualizationModel* model);
		void setValidationModel(ScheduleValidationModel* model);
		
		ScheduleActualizationModel* getActualizationModel() const;
		ScheduleValidationModel* getValidationModel() const;

		const TimeWindow& getShift() const;
		void setShift(const TimeWindow &shift);

		void clear();

		const Scene* getScene() const;
		Scene* getScene();

		const ScheduleConstraints& constraints() const;
		ScheduleConstraints& constraints();

		// == framework internal ====================================
		void setScene(Scene* scene);
		void setRunsFactory(SceneObjectsFactory<Run> *factory);
		void setStopsFactory(SceneObjectsFactory<WorkStop> *factory);

		void setRunVehicleBinder(RunVehicleBinder *run_vehicle_binder);

		void invalidateArrivalTimes();

	private:
        size_t id;
        std::string name;

        const Performer* performer;

		std::vector<Run*> runs;
		StopsList stops;

		Scene* scene;

		SceneObjectsFactory<Run> *runs_factory;
		SceneObjectsFactory<WorkStop> *stops_factory;

		Location depot_location;

		TimeWindow shift;

		ScheduleActualizationModel* schedule_actualization_model;
		ScheduleValidationModel* schedule_validation_model;

		RunVehicleBinder* run_vehicle_binder;

		ScheduleConstraints schedule_constraints;
    };
}
