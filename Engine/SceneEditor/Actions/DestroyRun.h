#pragma once

#include "../Action.h"
#include <boost/optional.hpp>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Utils/Optional.h>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT DestroyRun : public Action<Schedule::RunIterator>
	{
	public:
		DestroyRun(Schedule& schedule, Schedule::ConstRunIterator pos);
		virtual ~DestroyRun();
		
		virtual Schedule::RunIterator perform() override;
		virtual void rollback() override;
		
	private:
		Schedule& schedule;
		Schedule::ConstRunIterator pos;
		Schedule::RunDestructor destructor;
		Run& detached_run;
		Optional<Schedule::RunIterator> next_iterator;
		bool performed;
	};
}