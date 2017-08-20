#pragma once

#include "../Action.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/Utils/Optional.h>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT AllocateOrder : public Action<Run::WorkStopIterator>
	{
	public:
		AllocateOrder(Run& run, Run::ConstWorkStopIterator pos, const Order& order);
		
		virtual Run::WorkStopIterator perform() override;
		virtual void rollback() override;
		
	private:
                bool performed;
		Run& run;
		Run::ConstWorkStopIterator pos;
		const Order& order;
		Run::WorkStopIterator result_work_stop;
	};
}
