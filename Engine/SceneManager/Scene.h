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

namespace Scheduler
{
    class Schedule;
	class Run;
	class WorkStop;
	class SceneContext;

    class SCENEMANAGER_EXPORT Scene
    {
    public:
        Scene(size_t id, SceneContext* context);
        ~Scene();

        size_t getId() const;

		const ImmutableVector<Schedule*>& getSchedules() const;
        ImmutableVector<Schedule*>& getSchedules();

        Schedule* createSchedule(const Performer *performer);

    private:
        size_t id;

		SceneContext* context;
		
        std::vector<Schedule*> schedules;

		SceneObjectsFactory<Schedule> schedules_factory;
		SceneObjectsFactory<Run> runs_factory;
		SceneObjectsFactory<WorkStop> stops_factory;
    };
}
