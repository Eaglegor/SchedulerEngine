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

	PoolMemoryStore* MemoryManager::createPoolMemoryStore(size_t chunk_size, size_t initial_capacity)
	{
		void* mem = internal_malloc_memory_store.allocate(sizeof(PoolMemoryStore));
		PoolMemoryStore* store = new(mem) PoolMemoryStore(&internal_malloc_memory_store, chunk_size, initial_capacity);
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
