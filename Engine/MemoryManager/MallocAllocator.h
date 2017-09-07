#pragma once

#include "MemoryManager.h"

#include <iostream>

namespace Scheduler
{
	/** 
	 * @ingroup memory_manager
	 * 
	 * @brief This class represents the memory allocator suitable for the STL containers using the provided MallocMemoryStore to perform allocations
	 */
	template <typename T>
	class MallocAllocator
	{
	public:
		typedef T value_type;

		/** 
		 * @brief Default constructor
		 */
		MallocAllocator( )
		    : memory_store(MemoryManager::createMallocMemoryStore( ))
		{
		}

		/** 
		 * @brief Copy constructor
		 * 
		 * @param rhs Another allocator
		 */
		template <typename U>
		MallocAllocator(const MallocAllocator<U>& rhs)
		{
			this->memory_store = MemoryManager::createMallocMemoryStore( );
		}

		~MallocAllocator( )
		{
			MemoryManager::destroyMemoryStore(memory_store);
		}

		/** 
		 * @brief Allocates the memory block suitable to hold n instances of class T
		 * 
		 * @param n Count of T instances to allocate the memory for
		 * 
		 * @return The pointer to the begin of allocated memory block
		 */
		T* allocate(size_t n)
		{
			void* mem = memory_store->allocate(sizeof(T) * n);
			return static_cast<T*>(mem);
		}

		/** 
		 * @brief Releases the previously allocated memory
		 * 
		 * @param obj Pointer to the previously allocated memory
		 * @param n The count of T instances this memory was previously allocated
		 */
		void deallocate(T* obj, size_t n)
		{
			memory_store->deallocate(obj);
		}

	private:
		MallocMemoryStore* memory_store;
	};
}