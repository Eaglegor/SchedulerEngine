#pragma once

#include <cstddef>

namespace Scheduler
{
	class MemoryStore
	{
	public:
		virtual ~MemoryStore( ){};

		virtual void* allocate(size_t memory_size) = 0;
		virtual void deallocate(void* ptr)         = 0;
	};
}