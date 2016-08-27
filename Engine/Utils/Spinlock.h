#pragma once
#include <atomic>

namespace Scheduler
{
	class Spinlock {
		public:
			Spinlock() : state_(Unlocked) {}

			void lock()
			{
				while (state_.exchange(Locked, std::memory_order_acquire) == Locked);
			}
			
			void unlock()
			{
				state_.store(Unlocked, std::memory_order_release);
			}
		private:
			typedef enum {Locked, Unlocked} LockState;
			std::atomic<LockState> state_;
	};
}