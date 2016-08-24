#include "PoolMemoryStore.h"

#include <assert.h>
#include <algorithm>

namespace Scheduler
{
	PoolMemoryStore::PoolMemoryStore(std::size_t chunk_size, std::size_t initial_capacity):
	chunk_size(chunk_size),
	storage(chunk_size, initial_capacity)
	{
	}
	
	PoolMemoryStore::~PoolMemoryStore()
	{
		storage.purge_memory();
	}

	void* PoolMemoryStore::allocate(size_t memory_size)
	{
		assert(memory_size <= chunk_size);
		if(memory_size > chunk_size) return nullptr;
		void* mem = storage.malloc();
		return mem;
	}

	void PoolMemoryStore::deallocate(void* ptr)
	{
		storage.free(ptr);
	}
}
