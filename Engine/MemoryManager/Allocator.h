#pragma once

#include "MemoryManager.h"

namespace Scheduler
{
	template<typename T>
	class Allocator
	{
	public:
		typedef T value_type;

		Allocator(MemoryManager* memory_manager) : memory_manager(memory_manager) {}

		template<typename U>
		Allocator(const Allocator<U>& rhs)
		{
			this->memory_manager = rhs.getMemoryManager();
		}

		T* allocate(size_t n)
		{
			void* mem = memory_manager->allocate(sizeof(T) * n);
			return static_cast<T*>(mem);
		}

		void deallocate(T* obj, size_t n)
		{
			memory_manager->deallocate(obj);
		}

		MemoryManager* getMemoryManager() const
		{
			return memory_manager;
		}

	private:
		MemoryManager* memory_manager;
	};
}