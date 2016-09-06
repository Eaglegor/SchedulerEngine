#pragma once

#include <cstddef>
#include <deque>
#include <memory>
#include <Engine/MemoryManager/MallocAllocator.h>
#include <assert.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <SceneEditor_export.h>

namespace Scheduler
{
	class Action;
	
	class SCENEEDITOR_EXPORT Patch
	{
	public:
		enum class State
		{
			NOT_INITIALIZED,
			OPEN,
			HELD
		};
		
		Patch();
		Patch(const Patch &rhs) = delete;
		Patch(Patch &&rhs);
		~Patch();
		
		bool operator=(Patch &&rhs);
		
		template<typename ActionType, typename... Args>
		void performAction(Args&& ...args)
		{
			assert(state == State::OPEN);
			if(state != State::OPEN) {
				STATIC_LOG_ERROR("Patch", "Trying to perform action while patch is in invalid state: {} instead of {}", static_cast<int>(state), static_cast<int>(State::OPEN));
				return;
			}
			auto action = std::allocate_shared<ActionType>(MallocAllocator<ActionType>(memory_manager), std::forward<Args>(args)...);
			action->perform();
			actions.emplace_back(action);
			++current_version;
		}
		
		std::size_t getBaseVersion();
		std::size_t getCurrentVersion();
		State getState();
		
		void initialize(MemoryManager* memory_manager, std::size_t base_version);

		void hold();
		void recall();
		
	private:
		friend class Checkpoint;
		
		State state;
		std::size_t base_version;
		std::size_t current_version;
		MemoryManager* memory_manager;
		std::deque<std::shared_ptr<Action>> actions;
	};
}