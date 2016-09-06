#pragma once

#include <memory>
#include <deque>
#include <Engine/MemoryManager/MallocAllocator.h>
#include "Action.h"
#include "Patch.h"

namespace Scheduler
{
	class Checkpoint
	{
	public:
		Checkpoint(MemoryManager* memory_manager, std::size_t base_version) :
		memory_manager(memory_manager), 
		base_version(base_version),
		current_version(base_version)
		{}

		template<typename ActionType, typename... Args>
		void performAction(Args&& ...args)
		{
			auto action = std::allocate_shared<ActionType>(MallocAllocator<ActionType>(memory_manager), std::forward<Args>(args)...);
			action->perform();
			actions.emplace_back(action);
			++current_version;
		}

		void applyPatch(Patch&& patch);
		
		void rollback();
		void commit();

		std::size_t getBaseVersion();
		std::size_t getCurrentVersion();
		
	private:
		std::size_t base_version;
		std::size_t current_version;
		MemoryManager* memory_manager;

		std::deque<std::shared_ptr<Action>> actions;
	};
}
