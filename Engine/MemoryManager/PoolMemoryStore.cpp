#include "PoolMemoryStore.h"

#include <assert.h>
#include <algorithm>

namespace Scheduler
{
	PoolMemoryStore::PoolMemoryStore(MallocMemoryStore* backend_memory_store, size_t chunk_size, size_t initial_capacity):
		backend_memory_store(backend_memory_store),
		chunk_size(chunk_size),
		current_capacity(initial_capacity),
		capacity_left(current_capacity),
		free_list(nullptr)
	{
		assert(initial_capacity > 1);
		void* block = allocateNewBlock(initial_capacity);
		initFreeList(block, current_capacity);
	}

	PoolMemoryStore::~PoolMemoryStore()
	{
		for(void* ptr : allocated_blocks)
		{
			backend_memory_store->deallocate(ptr);
		}
	}

	void* PoolMemoryStore::allocate(size_t memory_size)
	{
		if (free_list == nullptr)
		{
			return nullptr;
		}

		if(capacity_left == 1)
		{
			size_t new_block_capacity = calculateNewBlockCapacity();
			void* block = allocateNewBlock(new_block_capacity);
			initFreeList(block, new_block_capacity);
			current_capacity += new_block_capacity;
			capacity_left += new_block_capacity;
		}

		void* ptr = free_list;

		free_list = static_cast<void**>(*free_list);

		--capacity_left;

		return ptr;
	}

	void PoolMemoryStore::deallocate(void* ptr)
	{
		*static_cast<void**>(ptr) = free_list;
		free_list = static_cast<void**>(ptr);

		++capacity_left;
	}

	void* PoolMemoryStore::allocateNewBlock(size_t capacity)
	{
		void* block = backend_memory_store->allocate(capacity * chunk_size);
		allocated_blocks.push_back(block);
		return block;
	}

	void PoolMemoryStore::initFreeList(void* list_start, size_t capacity)
	{
		void** ptr = static_cast<void**>(list_start);

		void** head = ptr;

		for (size_t i = 0; i < capacity - 1; ++i)
		{
			*ptr = reinterpret_cast<char*>(ptr) + chunk_size;
			size_t diff = reinterpret_cast<size_t>(*ptr) - reinterpret_cast<size_t>(ptr);
			ptr = static_cast<void**>(*ptr);
		}

		size_t final_diff = ptr - head;

		*ptr = nullptr;

		if(free_list)
		{
			*free_list = head;
		}
		else
		{
			free_list = head;
		}
	}

	size_t PoolMemoryStore::calculateNewBlockCapacity() const
	{
		size_t new_size = static_cast<size_t>(current_capacity * 1.5);
		return std::max<size_t>(new_size, 1);
	}
}
