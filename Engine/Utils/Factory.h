#pragma once

#include <type_traits>
#include <utility>
#include <cstddef>

#include <assert.h>

#include <Engine/MemoryManager/MemoryManager.h>
#include <Engine/MemoryManager/PoolMemoryStore.h>

namespace Scheduler
{
    template<typename BaseClass>
    class Factory
    {
    public:
		Factory(MemoryManager* memory_manager, size_t max_object_size, size_t initial_capacity) :
			memory_manager(memory_manager),
			memory_store(memory_manager->createPoolMemoryStore(max_object_size, initial_capacity)),
			max_object_size(max_object_size)
		{
			assert(initial_capacity > 0);
		}

		~Factory()
		{
			memory_manager->destroyMemoryStore(memory_store);
		}

        template<typename DerivedClass, typename... Args>
		DerivedClass* createObject(Args&&...args)
        {
            static_assert(std::is_base_of<BaseClass, DerivedClass>::value, "Requested object type is incompatible with factory base type");
			assert(sizeof(DerivedClass) <= max_object_size);
			if (sizeof(DerivedClass) > max_object_size) return nullptr;
			void* memory = memory_store->allocate(sizeof(DerivedClass));
            return new(memory) DerivedClass(std::forward<Args>(args)...);
        }

        void destroyObject(BaseClass* object)
        {
			object->~BaseClass();
			memory_store->deallocate(object);
        }

	private:
		size_t max_object_size;

		PoolMemoryStore* memory_store;
		MemoryManager* memory_manager;
    };

}