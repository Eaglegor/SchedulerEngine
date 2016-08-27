#include "SceneEditor.h"

namespace Scheduler
{
	SceneEditor::SceneEditor():
	current_version(0)
	{
		checkpoint();
	}

	SceneEditor::~SceneEditor()
	{
	}

	void SceneEditor::rollbackAll()
	{
		rollbackToCheckpoint(0);
		current_version = getCurrentCheckpoint()->getCurrentVersion();
	}

	void SceneEditor::rollbackToCheckpoint(size_t checkpoint_id)
	{
		while(checkpoints.size() - 1 > checkpoint_id)
		{
			checkpoints.back()->rollback();
			checkpoints.pop_back();
		}
		checkpoints.back()->rollback();
		current_version = getCurrentCheckpoint()->getCurrentVersion();
	}
	
	void SceneEditor::rollbackToLastCheckpoint()
	{
		checkpoints.back()->rollback();
		current_version = getCurrentCheckpoint()->getCurrentVersion();
	}

	void SceneEditor::commit()
	{
		while(checkpoints.size() > 1)
		{
			checkpoints.pop_front();
		}
		checkpoints.front()->commit();
		current_version = getCurrentCheckpoint()->getCurrentVersion();
	}

	Checkpoint* SceneEditor::getCurrentCheckpoint()
	{
		return checkpoints.back().get();
	}

	std::size_t SceneEditor::checkpoint()
	{
		checkpoints.emplace_back(std::allocate_shared<Checkpoint>(MallocAllocator<Checkpoint>(&memory_manager), &memory_manager, current_version));
		return checkpoints.size() - 1;
	}

	Patch SceneEditor::createPatch()
	{
		Patch p;
		p.initialize(&memory_manager, current_version);
		return std::move(p);
	}

	std::size_t SceneEditor::applyPatch(Patch&& patch)
	{
		std::size_t new_cp_id = checkpoint();
		Checkpoint* cp = getCurrentCheckpoint();
		cp->applyPatch(std::move(patch));
		current_version = cp->getCurrentVersion();
		return new_cp_id;
	}
	
	std::size_t SceneEditor::getCurrentVersion()
	{
		return current_version;
	}

}
