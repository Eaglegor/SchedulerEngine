#pragma once

#include <type_traits>
#include <utility>

#include <Engine/MemoryManager/MemoryManager.h>

namespace Scheduler
{
    template<typename BaseClass>
    class Factory
    {
    public:
		Factory(MemoryManager* memory_manager) : memory_manager(memory_manager)
		{}

        template<typename DerivedClass, typename... Args>
		DerivedClass* createObject(Args&&...args)
        {
            static_assert(std::is_base_of<BaseClass, DerivedClass>::value, "Requested object type is incompatible with factory base type");
			void* memory = memory_manager->allocate(sizeof(DerivedClass));
            return new(memory) DerivedClass(std::forward<Args>(args)...);
        }

        void destroyObject(BaseClass* object)
        {
			object->~BaseClass();
			memory_manager->deallocate(object);
        }

	private:
		MemoryManager* memory_manager;
    };
}