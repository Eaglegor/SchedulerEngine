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



namespace Scheduler
{
	/**
	 * @brief Class managing the transactional changes on the scenes
	 * 
	 * @details In most cases the algorithms remember initial value of cost function, then try to
	 * introduce a set of changes into the current solution, then check some acceptance condition and
	 * then either accept the solution or reject it reverting all changes they have made. In some cases
	 * the algoritms may also want to reject only the part of changes and to explore another branch of
	 * search space.
	 * 
	 * Such search process can be considered as transactional, i.e. we introduce a bunch of changes
	 * and then either commit this changes or roll them back. Scene editor takes the responsibility for these
	 * transactions management.
	 * 
	 * Each atomic change is encapsulated into the class implementing the Action interface. Scene editor provides
	 * the methods to perform this change, to create the checkpoints and to rollback the changes to some checkpoint.
	 * 
	 * It also provides the methods that allow to explore some different branches of search space and to apply the 
	 * best one (the branches are called "patches").
	 * 
	 * The changes are versioned so each change increments the current solution version and rollback restores
	 * the solution version to the value it had at the specified checkpoint.
	 * 
	 * Scene editor works with actions, each of which has it's own context so the scene editor itself isn't bound to
	 * the particular scene or schedule and can be used to modify multiple scenes in one transaction. 
	 * 
	 * @warning The transactions themselves are thread-unsafe and so:
	 * - No concurrent calls to the scene editor API are allowed
	 * - No concurrent modifications are allowed for the solution parts included into the transaction
	 * 
	 */
	class SceneEditor
	{
	public:
		/**
		 * @todo Move this class into the private section?
		 */
		enum class State
		{
			OPEN,
			PATCHING
		};

		/**
		 * @brief Constructor
		 * 
		 * @details This constructor creates the zero-indexed checkpoint that can be used to rollback all changes.
		 */
		SceneEditor( );

		/**
		 * @brief Destructor
		 * 
		 * @details This destructor destroys all checkpoints and actions. No actions are reverted. It can be considered as
		 * an implicit commit() call.
		 */
		~SceneEditor( );

		/**
		 * @brief Creates the checkpoint
		 * 
		 * @return Identifier of the created checkpoint
		 * 
		 * @sa SceneEditor::rollbackToCheckpoint
		 */
		std::size_t checkpoint( );

		/**
		 * @brief Reverts all changes done since the last commit() call 
		 * or from the scene editor creation if there were no commits.
		 */
		void rollbackAll( );

		/**
		 * @brief Reverts all changes done since the specified checkpoint creation
		 * 
		 * @param checkpoint_id The idendifier of the checkpoint
		 * 
		 * @sa SceneEditor::checkpoint
		 */
		void rollbackToCheckpoint(size_t checkpoint_id);

		/**
		 * @brief Reverts all changes done since the latest checkpoint creation
		 */
		void rollbackToLastCheckpoint( );

		/**
		 * @brief Commits all changes and clears the changes history
		 */
		void commit( );

		/**
		 * @brief Creates the scene patch
		 * 
		 * @details This method creates a subtree of changes history called "patch". It also changes
		 * the scene editor state to "PATCHING". When the scene editor is in this state the changes
		 * can only be applied through the patch interface.
		 * 
		 * @todo Take ownership of patches and return only the identifier exactly as it's done for checkpoints
		 * 
		 * @return The new patch
		 * 
		 * @sa SceneEditor::applyPatch, SceneEditor::abortPatching
		 */
		Patch createPatch( );

		/**
		 * @brief Applies the actions described by the specified patch
		 * 
		 * @details This method creates the new checkpoint, then merges all changes
		 * of the patch into the current changes tree and puts the scene editor into the
		 * "OPEN" state. 
		 * 
		 * @param patch Patch to apply
		 * 
		 * @return Identifier of checkpoint created before the patch application
		 */
		std::size_t applyPatch(Patch&& patch);

		/**
		 * @brief Aborts the patching process
		 * 
		 * @details This method puts scene editor into the "OPEN" state
		 */
		void abortPatching( );

		/**
		 * @brief Returns the current version of changes tree
		 * 
		 * @note If no modifications are done outside the scene editor, the same version of changes tree
		 * guarantees exactly the same state of scene (the same objects are contained in the same order).
		 * 
		 * @return The current version of changes tree
		 */
		std::size_t getCurrentVersion( );

		/**
		 * @brief Performs the specified action
		 * 
		 * @tparam ActionType The action type to perform
		 * @tparam Args Type of arguments of the action
		 * 
		 * @throw std::logic_error, if trying to perform action on the scene editor in "PATCHING" state
		 * 
		 * @param args Arguments of the action
		 * 
		 * @return The return value of action
		 */
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