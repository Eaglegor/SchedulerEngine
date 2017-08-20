#pragma once

#include "MemoryStore.h"

#include <MemoryManager_export.h>

namespace Scheduler
{
	class MEMORYMANAGER_EXPORT MallocMemoryStore : public MemoryStore
	{
	public:
		virtual void* allocate(size_t memory_size) override;
		virtual void deallocate(void* ptr) override;
	};
}