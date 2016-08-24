#include "MemoryManager.h"

#include <cstdlib>
#include <new>

#include "PoolMemoryStore.h"

namespace Scheduler
{
	MallocMemoryStore* MemoryManager::createMallocMemoryStore()
	{
		return &internal_malloc_memory_store;
	}

	PoolMemoryStore* MemoryManager::createPoolMemoryStore(size_t chunk_size, size_t initial_capacity, bool thread_safety_required)
	{
		void* mem = internal_malloc_memory_store.allocate(sizeof(PoolMemoryStore));
		PoolMemoryStore* store = new(mem) PoolMemoryStore(chunk_size, initial_capacity, thread_safety_required);
		return store;
	}

	void MemoryManager::destroyMemoryStore(PoolMemoryStore* memory_store)
	{
		memory_store->~PoolMemoryStore();
		internal_malloc_memory_store.deallocate(memory_store);
	}

	void MemoryManager::destroyMemoryStore(MallocMemoryStore* memory_store)
	{
	}
}
