#pragma once

#include "MemoryStore.h"
#include <list>
#include <boost/pool/pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <Engine/Utils/Spinlock.h>

#include <MemoryManager_export.h>

namespace Scheduler
{
	class MEMORYMANAGER_EXPORT PoolMemoryStore : public MemoryStore
	{
	public:
		PoolMemoryStore(std::size_t chunk_size, std::size_t initial_capacity);
		virtual ~PoolMemoryStore();
		
		virtual void* allocate(size_t memory_size) override;
		virtual void deallocate(void* ptr) override;

	private:
		const std::size_t chunk_size;
		boost::pool<boost::default_user_allocator_malloc_free> storage;
	};
}