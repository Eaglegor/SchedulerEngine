#pragma once

#include <assert.h>
#include <cstddef>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <unordered_set>
#include <Engine/SceneManager/Extensions/RunVehicleBinder.h>
#include "TemporarySchedule.h"

#include "SceneObjectsFactory.h"
#include <Engine/Utils/Factory.h>

#include <SceneManager_export.h>
#include "ScheduleActualizationModel.h"

#include "Constraints/Scene/SceneConstraints.h"
#include <boost/optional.hpp>
#include "Queries/SceneQueries.h"

#include "Listeners/StructuralChangesObserver.h"

namespace Scheduler
{
    class Schedule;
	class Run;
	class WorkStop;
	class SceneContext;
	class Performer;
	class SceneManager;
	class SceneQueries;

    class SCENEMANAGER_EXPORT Scene
    {
    public:
        Scene(std::size_t id, const SceneContext &context);
        ~Scene();

        std::size_t getId() const;

		const ImmutableVector<Schedule*>& getSchedules() const;
        ImmutableVector<Schedule*>& getSchedules();

        Schedule* createSchedule(const Performer &performer);
		
		const SceneContext& getContext() const;

		void setSceneManager(SceneManager* scene_manager);
		SceneManager* getSceneManager() const;
		
		bool isValid() const;
		
		void addStructuralChangesListener(StructuralChangesListener* listener);
		void removeStructuralChangesListener(StructuralChangesListener* listener);
		
		SceneQueries& query();
		
    private:
        std::size_t id;

		const SceneContext &context;
		
        std::vector<Schedule*> schedules;

		SceneObjectsFactory<Schedule> schedules_factory;
		SceneObjectsFactory<Run> runs_factory;
		SceneObjectsFactory<WorkStop> stops_factory;
		
		StructuralChangesObserver structural_changes_observer;
		
		SceneManager* scene_manager;
		
		boost::optional<SceneQueries> scene_queries;
    };
}
