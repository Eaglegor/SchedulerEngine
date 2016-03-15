#pragma once

#include <MemoryManager_export.h>

namespace Scheduler
{
	class MEMORYMANAGER_EXPORT MemoryManager
	{
	public:
		void* allocate(size_t size);
		void deallocate(void* ptr);
	};
}