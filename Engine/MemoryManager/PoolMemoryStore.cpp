#include "PoolMemoryStore.h"

#include <algorithm>
#include <assert.h>
#include <boost/pool/singleton_pool.hpp>

namespace Scheduler
{
	PoolMemoryStore::PoolMemoryStore(std::size_t chunk_size, std::size_t initial_capacity, bool enable_mutex)
	    : chunk_size(chunk_size),
	      enable_mutex(enable_mutex),
	      storage(chunk_size, initial_capacity)
	{
	}

	PoolMemoryStore::~PoolMemoryStore( )
	{
		storage.purge_memory( );
	}

	void* PoolMemoryStore::allocate(size_t memory_size)
	{
		//assert(memory_size <= chunk_size);
		if(memory_size > chunk_size) return nullptr;
		if(enable_mutex) mutex.lock( );
		void* mem = storage.malloc( );
		if(enable_mutex) mutex.unlock( );
		return mem;
	}

	void PoolMemoryStore::deallocate(void* ptr)
	{
		if(enable_mutex) mutex.lock( );
		storage.free(ptr);
		if(enable_mutex) mutex.unlock( );
	}
}
