#pragma once

#include "MemoryStore.h"



namespace Scheduler
{
	/** 
	 * @ingroup memory_manager
	 * 
	 * @brief This class represents the memory store allocating memory by calling std::malloc() and std::free()
	 */
	class MallocMemoryStore : public MemoryStore
	{
	public:
		/** 
		 * @brief Allocates memory by calling std::malloc()
		 * 
		 * @param memory_size Amount of memory in bytes to be allocated
		 * 
		 * @return Pointer to the allocated memory block
		 */
		virtual void* allocate(size_t memory_size) override;

		/** 
		 * @brief Releases memory by calling std::free()
		 * 
		 * @param ptr Pointer to the previously allocated memory block
		 */
		virtual void deallocate(void* ptr) override;
	};
}