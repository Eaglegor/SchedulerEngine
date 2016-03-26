#include "MallocMemoryStore.h"

#include <cstdlib>

namespace Scheduler
{
	void* MallocMemoryStore::allocate(size_t memory_size)
	{
		return std::malloc(memory_size);
	}

	void MallocMemoryStore::deallocate(void* ptr)
	{
		std::free(ptr);
	}
}
