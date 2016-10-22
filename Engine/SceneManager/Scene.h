#pragma once

#include <assert.h>
#include <cstddef>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <unordered_set>
#include "Algorithms/RunVehicleBinder.h"

#include "SceneObjectsFactory.h"
#include <Engine/Utils/Factory.h>

#include "ScheduleActualizationModel.h"
#include "Constraints/Scene/SceneConstraints.h"
#include <boost/optional.hpp>
#include "Queries/SceneQueries.h"

#include "Listeners/StructuralChangesObserver.h"

#include <SceneManager_export.h>

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
		using SchedulesList = std::vector<ReferenceWrapper<Schedule>>;
		
        Scene(std::size_t id, const SceneContext &context, SceneManager& scene_manager);
        ~Scene();

        std::size_t getId() const;

		const SchedulesList& getSchedules() const;
        SchedulesList& getSchedules();

        Schedule& createSchedule(const Performer &performer);
		
		const SceneContext& getContext() const;

		SceneManager& getSceneManager() const;
		
		bool isValid() const;
		
		void addStructuralChangesListener(StructuralChangesListener& listener);
		void removeStructuralChangesListener(StructuralChangesListener& listener);
		
		const SceneQueries& query() const;
		
		bool operator==(const Scene& rhs) const;
		bool operator!=(const Scene& rhs) const;
		
    private:
        std::size_t id;

		const SceneContext &context;
		
        SchedulesList schedules;

		SceneObjectsFactory<Schedule> schedules_factory;
		SceneObjectsFactory<Run> runs_factory;
		SceneObjectsFactory<WorkStop> stops_factory;
		
		StructuralChangesObserver structural_changes_observer;
		
		SceneManager& scene_manager;
		
		mutable Optional<SceneQueries> scene_queries;
    };
}
