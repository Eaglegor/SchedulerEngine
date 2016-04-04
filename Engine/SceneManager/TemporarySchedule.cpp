#include "TemporarySchedule.h"

#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	void TemporaryScheduleDeleter::operator()(Schedule* schedule) const
	{
		assert(scene_objects_factory || !schedule);
		if (scene_objects_factory) scene_objects_factory->destroyObject(schedule);
	}
}