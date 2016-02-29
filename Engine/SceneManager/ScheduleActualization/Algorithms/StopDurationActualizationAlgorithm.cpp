#include "StopDurationActualizationAlgorithm.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Views/ScheduleStopsView.h>
#include <Engine/Concepts/Basic/Duration.h>

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

	void StopDurationActualizationAlgorithm::onStopRemoved(const Run *run)
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

	void StopDurationActualizationAlgorithm::actualize()
	{
		if(!dirty_flag) return;

		ScheduleStopsView stops(schedule);

		for(Stop* stop : stops)
		{
			Duration total_duration;

			for(const Operation* operation : stop->getOperations())
			{
				total_duration += operation->getDuration();
			}
		}

		dirty_flag = false;
	}

	ScheduleActualizationAlgorithmType StopDurationActualizationAlgorithm::getType()
	{
		return type;
	}
}