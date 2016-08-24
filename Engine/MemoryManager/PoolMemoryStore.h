#pragma once

#include "MemoryStore.h"
#include <list>
#include <boost/pool/pool.hpp>
#include <boost/thread/mutex.hpp>

#include <MemoryManager_export.h>

namespace Scheduler
{
	class MEMORYMANAGER_EXPORT PoolMemoryStore : public MemoryStore
	{
	public:
		PoolMemoryStore(std::size_t chunk_size, std::size_t initial_capacity, bool enable_mutex);
		virtual ~PoolMemoryStore();
		
		virtual void* allocate(size_t memory_size) override;
		virtual void deallocate(void* ptr) override;

	private:
		const std::size_t chunk_size;
		const bool enable_mutex;
		boost::pool<boost::default_user_allocator_malloc_free> storage;
		boost::mutex mutex;
	};
}