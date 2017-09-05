#pragma once

#include <Engine/Utils/Optional.h>


namespace Scheduler
{
	class Schedule;
	class Scene;

	class ScheduleVariant
	{
	public:
		ScheduleVariant( );
		ScheduleVariant(Schedule& schedule);
		ScheduleVariant(ScheduleVariant&& rhs);
		~ScheduleVariant( );

		ScheduleVariant& operator=(ScheduleVariant&& rhs);
		ScheduleVariant& operator=(Schedule& rhs);

		Optional<Schedule&> getSchedule( ) const;
		void apply( ) const;

		operator bool( ) const;

	private:
		Optional<Schedule&> original_schedule;
		Optional<Schedule&> new_schedule;
		Optional<Scene&> new_scene;
	};
}