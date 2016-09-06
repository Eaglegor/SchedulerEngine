#pragma once

#include <cstddef>
#include <utility>
#include "Action.h"
#include "Checkpoint.h"
#include <Engine/MemoryManager/MemoryManager.h>
#include <deque>
#include "Patch.h"
#include <Engine/LoggingManager/LoggingManager.h>

#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT SceneEditor
	{
	public:
		enum class State
		{
			OPEN,
			PATCHING
		};
		
		SceneEditor();
		~SceneEditor();

		std::size_t checkpoint();

		void rollbackAll();
		void rollbackToCheckpoint(size_t checkpoint_id);
		void rollbackToLastCheckpoint();
		void commit();

		Patch createPatch();
		std::size_t applyPatch(Patch &&patch);
		void abortPatching();

		std::size_t getCurrentVersion();
		
		template<typename ActionType, typename... Args>
		void performAction(Args&& ...args)
		{
			if(state != SceneEditor::State::OPEN)
			{
				STATIC_SIMPLE_LOG_ERROR("SceneEditor", "Trying to perform an action when in patching state");
				return;
			}
			Checkpoint* current_checkpoint = getCurrentCheckpoint();
			current_checkpoint->performAction<ActionType>(std::forward<Args>(args)...);
			current_version = current_checkpoint->getCurrentVersion();
		}

	private:
		std::size_t current_version;
		Checkpoint* getCurrentCheckpoint();

		MemoryManager memory_manager;
		std::deque<std::shared_ptr<Checkpoint>> checkpoints;
		
		State state;
	};
}