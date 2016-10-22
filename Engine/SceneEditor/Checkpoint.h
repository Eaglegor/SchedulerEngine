#pragma once

#include <memory>
#include <deque>
#include <Engine/MemoryManager/MallocAllocator.h>
#include "Action.h"
#include "Patch.h"

#include <SceneEditor_export.h>

namespace Scheduler
{
	class SCENEEDITOR_EXPORT Checkpoint
	{
	public:
		explicit Checkpoint(std::size_t base_version) :
		base_version(base_version),
		current_version(base_version)
		{}

		template<typename ActionType, typename... Args>
		typename ActionType::return_type performAction(std::size_t &out_version, Args&& ...args)
		{
			auto action = std::allocate_shared<ActionType>(MallocAllocator<ActionType>(), std::forward<Args>(args)...);
			actions.emplace_back(action);
			++current_version;
			out_version = current_version;
			return action->perform();
		}

		void applyPatch(Patch&& patch);
		
		void rollback();
		void commit();

		std::size_t getBaseVersion();
		std::size_t getCurrentVersion();
		
	private:
		std::size_t base_version;
		std::size_t current_version;

		std::deque<std::shared_ptr<ActionBase>> actions;
	};
}
