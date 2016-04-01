#include "TemporarySchedule.h"
#include "Schedule.h"

namespace Scheduler
{
	TemporarySchedule::TemporarySchedule(Schedule * schedule, SceneObjectsFactory<Schedule>* schedules_factory):
		schedule(schedule),
		schedules_factory(schedules_factory)
	{
	}

	TemporarySchedule::~TemporarySchedule()
	{
		schedules_factory->destroyObject(schedule);
	}

	Schedule* TemporarySchedule::operator->()
	{
		return schedule;
	}

	const Schedule* TemporarySchedule::operator->() const
	{
		return schedule;
	}
}