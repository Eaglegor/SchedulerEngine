#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Utils/Optional.h>
#include <SceneEditor_export.h>
#include <boost/optional.hpp>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT CreateRun : public Action<Schedule::RunIterator>
	{
	public:
		CreateRun(Schedule& schedule, Schedule::ConstRunIterator pos, const Location& from, const Location& to);

		virtual Schedule::RunIterator perform( ) override;
		virtual void rollback( ) override;

	private:
		Schedule& schedule;
		Schedule::ConstRunIterator pos;
		const Location& from;
		const Location& to;
		Optional<Schedule::RunIterator> resulting_run;
	};
}