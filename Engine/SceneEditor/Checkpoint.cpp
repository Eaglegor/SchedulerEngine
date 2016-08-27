#include "Checkpoint.h"
#include "assert.h"
#include <algorithm>

namespace Scheduler
{
	void Checkpoint::rollback()
	{
		for (auto iter = actions.rbegin(); iter != actions.rend(); ++iter)
		{
			iter->get()->rollback();
		}
		actions.clear();
		current_version = base_version;
	}

	void Checkpoint::commit()
	{
		actions.clear();
		base_version = current_version;
	}
	
	std::size_t Checkpoint::getBaseVersion()
	{
		return base_version;
	}
	
	std::size_t Checkpoint::getCurrentVersion()
	{
		return current_version;
	}

	void Checkpoint::applyPatch(Patch&& patch)
	{
		if(patch.getState() == Patch::State::NOT_INITIALIZED) return;
		
		assert(patch.getBaseVersion() == getBaseVersion());
		if(patch.getBaseVersion() != getBaseVersion()) return;
		
		if(patch.getState() == Patch::State::HELD) patch.recall();
		
		std::move_backward(patch.actions.begin(), patch.actions.end(), actions.end());
		current_version = patch.getCurrentVersion();
	}
}