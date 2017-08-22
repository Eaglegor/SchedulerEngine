#pragma once
#include <atomic>

namespace Scheduler
{
	/** 
	 * @brief Fast synchronization primitive based on CAS operation
	 */
	class Spinlock
	{
	public:
		/** 
		 * @brief Default constructor
		 */
		Spinlock( )
		    : state_(Unlocked)
		{
		}

		/** 
		 * @brief Waits for the primitive is unlocked and locks the primitive
		 */
		void lock( )
		{
			while(state_.exchange(Locked, std::memory_order_acquire) == Locked)
				;
		}

		/** 
		 * @brief Unlocks the primitive
		 */
		void unlock( )
		{
			state_.store(Unlocked, std::memory_order_release);
		}

	private:
		typedef enum { Locked,
			           Unlocked } LockState;
		std::atomic<LockState> state_;
	};
}