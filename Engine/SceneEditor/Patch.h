#pragma once

#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/MemoryManager/MallocAllocator.h>

#include <assert.h>
#include <cstddef>
#include <deque>
#include <memory>

namespace Scheduler
{
	class ActionBase;

	class Patch
	{
	public:
		enum class State
		{
			NOT_INITIALIZED,
			OPEN,
			HELD
		};

		Patch( );
		Patch(const Patch& rhs) = delete;
		Patch(Patch&& rhs);
		~Patch( );

		Patch& operator=(Patch&& rhs);

		template <typename ActionType, typename... Args>
		typename ActionType::return_type performAction(Args&&... args)
		{
			assert(state == State::OPEN);
			if(state != State::OPEN)
			{
				logger.error("Trying to perform action while patch is in invalid state: {} instead of {}", static_cast<int>(state), static_cast<int>(State::OPEN));
				throw std::logic_error("Trying to perform an action when patch is in invalid state");
			}
			auto action = std::allocate_shared<ActionType>(MallocAllocator<ActionType>( ), std::forward<Args>(args)...);
			actions.emplace_back(action);
			++current_version;
			return action->perform( );
		}

		std::size_t getBaseVersion( );
		std::size_t getCurrentVersion( );
		State getState( );

		void initialize(std::size_t base_version);

		void hold( );
		void recall( );

	private:
		friend class Checkpoint;

		State state;
		std::size_t base_version;
		std::size_t current_version;
		std::deque<std::shared_ptr<ActionBase>> actions;
		Logger& logger;
	};
}