#include "MemoryManager.h"

#include <cstdlib>

namespace Scheduler
{
	void* MemoryManager::allocate(size_t size)
	{
		return ::malloc(size);
	}

	void MemoryManager::deallocate(void* ptr)
	{
		::free(ptr);
	}
}
