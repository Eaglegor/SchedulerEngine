#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Optional.h>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT AllocateOrder : public Action<Optional<Run::WorkStopIterator>>
	{
	public:
		AllocateOrder(Run& run, Run::ConstWorkStopIterator pos, const Order& order);
		
		virtual Optional<Run::WorkStopIterator> perform() override;
		virtual void rollback() override;
		
	private:
		Run& run;
		Run::ConstWorkStopIterator pos;
		const Order& order;
		Optional<Run::WorkStopIterator> result_work_stop;
	};
}