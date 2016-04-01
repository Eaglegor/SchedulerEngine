#include "StopDurationActualizationAlgorithm.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Views/ScheduleStopsView.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/StopVisitor.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/RunBoundaryStop.h>

#include <assert.h>

namespace Scheduler
{

	StopDurationActualizationAlgorithm::StopDurationActualizationAlgorithm(Schedule *schedule):
	ScheduleActualizationAlgorithm(schedule),
	dirty_flag(true)
	{

	}

	void StopDurationActualizationAlgorithm::onOperationAdded(const Stop *stop, const Operation *operation)
	{
		dirty_flag = true;
	}

	void StopDurationActualizationAlgorithm::onOperationRemoved(const Stop *stop)
	{
		dirty_flag = true;
	}

	void StopDurationActualizationAlgorithm::onStopNextRouteChanged(const Stop *stop)
	{
		// Doesn't affect duration
	}

	void StopDurationActualizationAlgorithm::onStopAdded(const Run *run, const Stop *stop, size_t index)
	{
		dirty_flag = true;
	}

	void StopDurationActualizationAlgorithm::onStopReplaced(const Run * run, const Stop * new_stop, size_t index)
	{
		dirty_flag = true;
	}

	void StopDurationActualizationAlgorithm::onStopRemoved(const Run *run, size_t index)
	{
		dirty_flag = true;
	}

	void StopDurationActualizationAlgorithm::onRunVehicleChanged(const Run *run, const Vehicle *vehicle)
	{
		// Doesn't affect duration
	}

	void StopDurationActualizationAlgorithm::onRunAdded(const Run *run, size_t index)
	{
		// Doesn't affect duration
	}

	void StopDurationActualizationAlgorithm::onRunRemoved()
	{
		// Doesn't affect duration
	}

	class TotalOperationDurationGetter : public StopVisitor
	{
	public:
		TotalOperationDurationGetter(Duration &out_duration):out_duration(out_duration){}

		virtual void dispatch(WorkStop * work_stop) override
		{
			assert(work_stop->getOperation());

			out_duration = work_stop->getOperation()->getDuration();
		}

		virtual void dispatch(RunBoundaryStop * run_boundary_stop) override
		{
			Duration total_duration;
			for (const Operation* operation : run_boundary_stop->getOperations())
			{
				total_duration += operation->getDuration();
			}
			out_duration = total_duration;
		}

	private:
		Duration &out_duration;
	};

	void StopDurationActualizationAlgorithm::actualize()
	{
		if(!dirty_flag) return;

		if (schedule->getRuns().empty())
		{
			dirty_flag = false;
			return;
		}
		
		for (Stop* stop = (*schedule->getRuns().begin())->getStartStop(); stop != nullptr; stop = stop->getNextStop())
		{
			Duration operations_duration;
			TotalOperationDurationGetter duration_getter(operations_duration);
			stop->acceptVisitor(&duration_getter);
			stop->setDuration(operations_duration);
		}

		dirty_flag = false;
	}
	ScheduleActualizationAlgorithm * StopDurationActualizationAlgorithm::clone(Schedule* schedule, Factory<ScheduleActualizationAlgorithm>* factory)
	{
		return factory->createObject<StopDurationActualizationAlgorithm>(schedule);
	}
}