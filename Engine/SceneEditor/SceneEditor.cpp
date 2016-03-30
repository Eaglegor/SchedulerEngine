#include "SceneEditor.h"

namespace Scheduler
{
	SceneEditor::SceneEditor()
	{
		checkpoint();
	}

	SceneEditor::~SceneEditor()
	{
	}

	void SceneEditor::rollbackAll()
	{
		rollbackToCheckpoint(0);
	}

	void SceneEditor::rollbackToCheckpoint(size_t checkpoint_id)
	{
		while(checkpoints.size() - 1 > checkpoint_id)
		{
			checkpoints.back()->rollback();
			checkpoints.pop_back();
		}
		checkpoints.back()->rollback();
	}
	
	void SceneEditor::rollbackToLastCheckpoint()
	{
		checkpoints.back()->rollback();
	}

	void SceneEditor::commit()
	{
		while(checkpoints.size() > 1)
		{
			checkpoints.pop_back();
		}
		checkpoints.back()->clear();
	}

	Checkpoint* SceneEditor::getCurrentCheckpoint()
	{
		return checkpoints.back().get();
	}

	size_t SceneEditor::checkpoint()
	{
		checkpoints.emplace_back(std::allocate_shared<Checkpoint>(MallocAllocator<Checkpoint>(&memory_manager), &memory_manager));
		return checkpoints.size() - 1;
	}
}
