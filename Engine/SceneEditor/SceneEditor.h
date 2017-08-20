#pragma once

#include "Action.h"
#include "Checkpoint.h"
#include "Patch.h"
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/MemoryManager/MemoryManager.h>
#include <cstddef>
#include <deque>
#include <exception>
#include <utility>

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

		SceneEditor( );
		~SceneEditor( );

		std::size_t checkpoint( );

		void rollbackAll( );
		void rollbackToCheckpoint(size_t checkpoint_id);
		void rollbackToLastCheckpoint( );
		void commit( );

		Patch createPatch( );
		std::size_t applyPatch(Patch&& patch);
		void abortPatching( );

		std::size_t getCurrentVersion( );

		template <typename ActionType, typename... Args>
		typename ActionType::return_type performAction(Args&&... args)
		{
			static_assert(std::is_base_of<ActionBase, ActionType>::value, "Incorrect action type");

			if(state != SceneEditor::State::OPEN)
			{
				logger.error("Trying to perform an action when in patching state");
				throw std::logic_error("Trying to perform an action when in patching state");
			}
			Checkpoint* current_checkpoint = getCurrentCheckpoint( );
			return current_checkpoint->performAction<ActionType>(current_version, std::forward<Args>(args)...);
		}

	private:
		std::size_t current_version;
		Checkpoint* getCurrentCheckpoint( );

		std::deque<std::shared_ptr<Checkpoint>> checkpoints;

		State state;

		Logger& logger;
	};
}