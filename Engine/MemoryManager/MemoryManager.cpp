#include "MemoryManager.h"

#include <cstdlib>

namespace Scheduler
{
	void* MemoryManager::allocate(size_t size)
	{
		return std::malloc(size);
	}

	void MemoryManager::deallocate(void* ptr)
	{
		std::free(ptr);
	}
}
