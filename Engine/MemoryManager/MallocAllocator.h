#pragma once

#include "MemoryManager.h"

#include <iostream>

namespace Scheduler
{
	template <typename T>
	class MallocAllocator
	{
	public:
		typedef T value_type;

		MallocAllocator( )
		    : memory_store(MemoryManager::createMallocMemoryStore( ))
		{
		}

		template <typename U>
		MallocAllocator(const MallocAllocator<U>& rhs)
		{
			this->memory_store = MemoryManager::createMallocMemoryStore( );
		}

		~MallocAllocator( )
		{
			MemoryManager::destroyMemoryStore(memory_store);
		}

		T* allocate(size_t n)
		{
			void* mem = memory_store->allocate(sizeof(T) * n);
			return static_cast<T*>(mem);
		}

		void deallocate(T* obj, size_t n)
		{
			memory_store->deallocate(obj);
		}

	private:
		MallocMemoryStore* memory_store;
	};
}