#pragma once

#include "MallocMemoryStore.h"

#include <cstddef>

namespace Scheduler
{
	class MemoryStore;
	class PoolMemoryStore;

	/**
	 * @brief This class manages memory allocations inside the engine
	 * 
	 * @details {This class provides 2 memory allocation strategies: direct malloc call and pool allocator. To use the provided strategy,
	 * the user has to create the corresponding memory store (either MallocMemoryStore or PoolMemoryStore) and then request memory allocations
	 * over the returned MemoryStore instance}
	 * 
	 */
	class MemoryManager
	{
	public:
		/**
		 * @brief Creates malloc memory store
		 * 
		 * @details {Requesting memory from resulting memory store implies syscall to malloc(). Freeing memory implies syscall to free().
		 * Since the underlying memory store is completely stateless, the single shared instance pointer is always returned by this method}
		 * 
		 * @return Shared instance of MallocMemoryStore
		 */
		static MallocMemoryStore* createMallocMemoryStore( );

		/**
		 * @brief Creates pool memory store
		 * 
		 * @details {Resulting memory store uses the pre-allocated memory pool of fixed-sized chunks. When new memory allocation is requested,
		 * a pointer to the next free chunk is returned and the chunk is removed from the pool. When the memory is released, the chunk is returned
		 * to the pool and becomes available for the next memory allocations. The pool size starts from the passed initial capacity and automatically
		 * grows by allocating new memory areas if there are no more free chunks left.
		 * 
		 * The sizeof() for the objects constructed in the returned memory must be less than or equal to the chunk size.
		 * 
		 * By default the returned memory store isn't thread-safe but the thread-safety can be turned on. Generally the thread-safe version is slower
		 * since it uses the blocking synchnorization primitives, so, if it's possible, the concurrent calls to the single pool memory store should be 
		 * avoided and the thread-unsafe version should be used.
		 * 
		 * The calls to this method are thread-safe. 
		 * 
		 * Some predefined size constants are declared for convenience in the Pool namespace.
		 * }
		 * 
		 * @param chunk_size The size in bytes of a single fixed-sized memory chunk
		 * @param initial_capacity The count of fixed-sized chunks of memory to be pre-allocated on memory store creation
		 * @param thread_safety_required If true, the blocking synchronization primitives are used to make the allocations thread-safe
		 * 
		 * @return New instance of PoolMemoryStore
		 */
		static PoolMemoryStore* createPoolMemoryStore(size_t chunk_size, size_t initial_capacity, bool thread_safety_required = false);

		/**
		 * @brief This method does nothing and is provided for code compatibility
		 * 
		 * @param memory_store Memory store to be destroyed
		 */
		static void destroyMemoryStore(MallocMemoryStore* memory_store);

		/** 
		 * @brief Destroys the pool memory store freeing it's allocated memory
		 * 
		 * @param memory_store Memory store to be destroyed
		 */
		static void destroyMemoryStore(PoolMemoryStore* memory_store);

	private:
		static MallocMemoryStore internal_malloc_memory_store;
	};
}