#pragma once

#include <cstddef>
#include <utility>
#include "Action.h"
#include "Checkpoint.h"
#include <Engine/MemoryManager/MemoryManager.h>
#include <deque>

#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT SceneEditor
	{
	public:
		SceneEditor();
		~SceneEditor();

		size_t checkpoint();

		void rollbackAll();
		void rollbackToCheckpoint(size_t checkpoint_id);
		void rollbackToLastCheckpoint();
		void commit();

		template<typename ActionType, typename... Args>
		void performAction(Args&& ...args)
		{
			Checkpoint* current_checkpoint = getCurrentCheckpoint();
			current_checkpoint->performAction<ActionType>(std::forward<Args>(args)...);
		}

	private:
		Checkpoint* getCurrentCheckpoint();

		MemoryManager memory_manager;
		std::deque<std::shared_ptr<Checkpoint>> checkpoints;
	};
}