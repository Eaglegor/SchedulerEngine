#pragma once

#include "MemoryManager.h"

#include <iostream>

namespace Scheduler
{
	template<typename T>
	class MallocAllocator
	{
	public:
		typedef T value_type;

		MallocAllocator(MemoryManager* memory_manager) : 
			memory_manager(memory_manager),
			memory_store(memory_manager->createMallocMemoryStore())
		{
		}

		template<typename U>
		MallocAllocator(const MallocAllocator<U>& rhs)
		{
			this->memory_manager = rhs.getMemoryManager();
			this->memory_store = memory_manager->createMallocMemoryStore();
		}

		~MallocAllocator()
		{
			memory_manager->destroyMemoryStore(memory_store);
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

		MemoryManager* getMemoryManager() const
		{
			return memory_manager;
		}

	private:
		MemoryManager* memory_manager;
		MallocMemoryStore* memory_store;
	};
}