#pragma once

#include <cstddef>
#include "PoolMemoryStore.h"
#include <boost/pool/pool.hpp>


namespace Scheduler
{
	template<std::size_t chunk_size>
	class BoostPoolMemoryStore : public PoolMemoryStore
	{
	public:
		virtual void* allocate(std::size_t memory_size);
		virtual void deallocate(void* ptr);
		
	private:
		
	};
}