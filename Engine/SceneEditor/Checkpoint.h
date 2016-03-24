#pragma once

#include <memory>
#include <deque>
#include <Engine/MemoryManager/MallocAllocator.h>
#include "Action.h"

namespace Scheduler
{
	class Checkpoint
	{
	public:
		Checkpoint(MemoryManager* memory_manager) :memory_manager(memory_manager){}

		template<typename ActionType, typename... Args>
		void performAction(Args&& ...args)
		{
			auto action = std::allocate_shared<ActionType>(MallocAllocator<ActionType>(memory_manager), std::forward<Args>(args)...);
			action->perform();
			actions.emplace_back(action);
		}

		void rollback();
		void clear();

	private:
		MemoryManager* memory_manager;

		std::deque<std::shared_ptr<Action>> actions;
	};
}
