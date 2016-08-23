#include "DefaultDurationActualizationAlgorithm.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/StopVisitor.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/RunBoundaryStop.h>

#include <assert.h>

namespace Scheduler
{
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

	void DefaultDurationActualizationAlgorithm::actualize(Run* run)
	{
		for(Stop* stop : run->getStops())
		{
			Duration operations_duration;
			TotalOperationDurationGetter duration_getter(operations_duration);
			stop->acceptVisitor(&duration_getter);
			stop->setDuration(operations_duration);
		}
	}
}
