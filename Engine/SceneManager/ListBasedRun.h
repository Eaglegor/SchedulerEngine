#pragma once

#include "Run.h"
#include <Engine/Utils/Collections/ImmutableList.h>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Schedule;
	class Vehicle;
	class WorkStop;
	class RoutingService;
	class RoutingProfile;
	class ScheduleActualizer;

	class SCENEMANAGER_EXPORT ListBasedRun : public Run
	{
	public:
		ListBasedRun(size_t id, const Location& start_location, const Location& end_location, Schedule* schedule);
		~ListBasedRun();

		const ImmutableList<WorkStop*>& getWorkStops() const;
		ImmutableList<WorkStop*>& getWorkStops();

		WorkStop* allocateWorkOperation(const Operation *operation, ImmutableList<WorkStop*>::iterator iter);
		WorkStop* allocateWorkOperation(const Operation *operation);

		void unallocateWorkOperation(ImmutableList<WorkStop*>::iterator iter);

		Stop* replaceWorkOperation(ImmutableList<WorkStop*>::iterator iter, const Operation* new_operation);

	private:
		std::list<WorkStop*> work_stops;

		virtual void invalidateRoutes() override;
		void invalidateWorkStopRoutes(ImmutableList<WorkStop*>::iterator iter);
	};
}