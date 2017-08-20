#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

#include <assert.h>

#include <Engine/MemoryManager/MemoryManager.h>
#include <Engine/MemoryManager/PoolMemoryStore.h>

namespace Scheduler
{
	template <typename BaseClass>
	class Factory
	{
	public:
		Factory(size_t max_object_size, size_t initial_capacity = 10)
		    : memory_store(MemoryManager::createPoolMemoryStore(max_object_size, initial_capacity)),
		      max_object_size(max_object_size)
		{
			assert(initial_capacity > 0);
		}

		~Factory( )
		{
			MemoryManager::destroyMemoryStore(memory_store);
		}

		template <typename DerivedClass, typename... Args>
		DerivedClass* createObject(Args&&... args)
		{
			static_assert(std::is_base_of<BaseClass, DerivedClass>::value, "Requested object type is incompatible with factory base type");
			assert(sizeof(DerivedClass) <= max_object_size);
			if(sizeof(DerivedClass) > max_object_size) return nullptr;
			void* memory = memory_store->allocate(sizeof(DerivedClass));
			return new(memory) DerivedClass(std::forward<Args>(args)...);
		}

		void destroyObject(BaseClass* object)
		{
			object->~BaseClass( );
			memory_store->deallocate(object);
		}

	private:
		size_t max_object_size;

		PoolMemoryStore* memory_store;
	};
}