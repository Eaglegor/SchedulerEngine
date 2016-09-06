#pragma once

#include <cstddef>
#include "MallocMemoryStore.h"
#include <MemoryManager_export.h>

namespace Scheduler
{
	class MemoryStore;
	class PoolMemoryStore;

	class MEMORYMANAGER_EXPORT MemoryManager
	{
	public:
		static MallocMemoryStore* createMallocMemoryStore();
		static PoolMemoryStore* createPoolMemoryStore(size_t chunk_size, size_t initial_capacity, bool thread_safety_required = false);
		
		static void destroyMemoryStore(MallocMemoryStore* memory_store);
		static void destroyMemoryStore(PoolMemoryStore* memory_store);

	private:
		static MallocMemoryStore internal_malloc_memory_store;
	};
}