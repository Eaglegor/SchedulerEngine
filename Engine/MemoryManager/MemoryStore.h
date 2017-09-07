#pragma once

#include <cstddef>

namespace Scheduler
{
	/** 
	 * @ingroup memory_manager
	 * 
	 * @brief This class represents the abstract memory store interface
	 * 
	 * @details Memory store is a class that provides the methods to allocate and release memory blocks
	 */
	class MemoryStore
	{
	public:
		virtual ~MemoryStore( ){};

		virtual void* allocate(size_t memory_size) = 0;
		virtual void deallocate(void* ptr)         = 0;
	};
}