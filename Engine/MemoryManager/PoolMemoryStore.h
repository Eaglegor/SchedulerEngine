#pragma once

#include "MemoryStore.h"
#include <boost/pool/pool.hpp>
#include <boost/thread/mutex.hpp>
#include <list>



namespace Scheduler
{
	/** 
	 * @ingroup memory_manager
	 * 
	 * @brief This class represents the memory store allocating memory by returning fixed-sized blocks stored in the memory pool
	 * 
	 * @details This class uses boost::pool as the underlying memory storage. Since boost::pool implementation isn't thread-safe,
	 * the external synchronization mechanism using boost::mutex is provided by this class.
	 */
	class PoolMemoryStore : public MemoryStore
	{
	public:
		/** 
		 * @brief Constructor
		 * 
		 * @param chunk_size The size of a fixed-sized memory chunk stored in the pool
		 * @param initial_capacity The initial count of memory chunks to pre-allocate
		 * @param enable_mutex If true, the external synchronization mechanism using boost::mutex is used to achieve thread-safety of allocations
		 */
		PoolMemoryStore(std::size_t chunk_size, std::size_t initial_capacity, bool enable_mutex);
		virtual ~PoolMemoryStore( );

		/** 
		 * @brief Allocates the next memory chunk if the requested memory amount fits into the fixed chunk size
		 * 
		 * @param memory_size Amount of memory to allocate the chunk for
		 * 
		 * @return Pointer to the allocated chunk or nullptr if the chunk size is less than memory_size
		 */
		virtual void* allocate(size_t memory_size) override;

		/** 
		 * @brief Releases the previously allocated chunk
		 * 
		 * @param ptr Pointer to the previously allocated chunk
		 * @note {Passing the pointer that wasn't allocated by this memory store causes the undefined behavior}
		 */
		virtual void deallocate(void* ptr) override;

	private:
		const std::size_t chunk_size;
		const bool enable_mutex;
		boost::pool<boost::default_user_allocator_malloc_free> storage;
		boost::mutex mutex;
	};
}