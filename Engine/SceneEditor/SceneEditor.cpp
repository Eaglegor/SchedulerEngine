#include "SceneEditor.h"

namespace Scheduler
{
	SceneEditor::SceneEditor( )
	    : current_version(0),
	      state(State::OPEN),
	      logger(LoggingManager::getLogger("SceneEditor"))
	{
		checkpoint( );
	}

	SceneEditor::~SceneEditor( )
	{
	}

	void SceneEditor::rollbackAll( )
	{
		if(state != SceneEditor::State::OPEN)
		{
			logger.error("Trying to perform rollback while in patching state");
			return;
		}

		rollbackToCheckpoint(0);
		current_version = getCurrentCheckpoint( )->getCurrentVersion( );
	}

	void SceneEditor::rollbackToCheckpoint(size_t checkpoint_id)
	{
		if(state != SceneEditor::State::OPEN)
		{
			logger.error("Trying to perform rollback while in patching state");
			return;
		}

		while(checkpoints.size( ) - 1 > checkpoint_id)
		{
			checkpoints.back( )->rollback( );
			checkpoints.pop_back( );
		}
		checkpoints.back( )->rollback( );
		current_version = getCurrentCheckpoint( )->getCurrentVersion( );
	}

	void SceneEditor::rollbackToLastCheckpoint( )
	{
		if(state != SceneEditor::State::OPEN)
		{
			logger.error("Trying to perform rollback while in patching state");
			return;
		}

		checkpoints.back( )->rollback( );
		current_version = getCurrentCheckpoint( )->getCurrentVersion( );
	}

	void SceneEditor::commit( )
	{
		while(checkpoints.size( ) > 1)
		{
			checkpoints.pop_front( );
		}
		checkpoints.front( )->commit( );
		current_version = getCurrentCheckpoint( )->getCurrentVersion( );
	}

	Checkpoint* SceneEditor::getCurrentCheckpoint( )
	{
		return checkpoints.back( ).get( );
	}

	std::size_t SceneEditor::checkpoint( )
	{
		checkpoints.emplace_back(std::allocate_shared<Checkpoint>(MallocAllocator<Checkpoint>( ), current_version));
		return checkpoints.size( ) - 1;
	}

	Patch SceneEditor::createPatch( )
	{
		Patch p;
		p.initialize(current_version);

		state = State::PATCHING;

		return std::move(p);
	}

	std::size_t SceneEditor::applyPatch(Patch&& patch)
	{
		if(state != State::PATCHING)
		{
			logger.error("Can't apply patch while not in patching state");
		}
		if(patch.getState( ) == Patch::State::NOT_INITIALIZED)
		{
			logger.error("Applying not initialized patch");
			return checkpoints.size( ) - 1;
		}
		if(current_version != patch.getBaseVersion( ))
		{
			logger.error("Incorrect patch base version. Scene editor version = {}, patch base version = {}", current_version, patch.getBaseVersion( ));
			return checkpoints.size( ) - 1;
		}
		std::size_t new_cp_id = checkpoint( );
		Checkpoint* cp        = getCurrentCheckpoint( );
		cp->applyPatch(std::move(patch));
		current_version = cp->getCurrentVersion( );

		state = State::OPEN;

		return new_cp_id;
	}

	std::size_t SceneEditor::getCurrentVersion( )
	{
		return current_version;
	}

	void SceneEditor::abortPatching( )
	{
		state = State::OPEN;
	}
}
