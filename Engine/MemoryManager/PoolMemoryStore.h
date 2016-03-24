#pragma once

#include "MemoryStore.h"
#include "MallocMemoryStore.h"
#include <list>

#include <MemoryManager_export.h>

namespace Scheduler
{
	class MEMORYMANAGER_EXPORT PoolMemoryStore : public MemoryStore
	{
	public:
		PoolMemoryStore(MallocMemoryStore* backend_memory_store, size_t chunk_size, size_t initial_capacity);
		~PoolMemoryStore();

		virtual void* allocate(size_t memory_size) override;
		virtual void deallocate(void* ptr) override;

	private:
		void* allocateNewBlock(size_t capacity);
		void initFreeList(void* list_start, size_t capacity);
		size_t calculateNewBlockCapacity() const;

		MallocMemoryStore* backend_memory_store;
		void** free_list;

		size_t chunk_size;
		size_t current_capacity;
		size_t capacity_left;

		std::list<void*> allocated_blocks;
	};
}