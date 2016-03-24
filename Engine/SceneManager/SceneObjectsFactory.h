#pragma once

#include <cstddef>
#include <Engine/Utils/Factory.h>

namespace Scheduler
{
    template<typename T>
    class SceneObjectsFactory
    {
    public:
		SceneObjectsFactory(MemoryManager* memory_manager, size_t initial_capacity = 100): 
			counter(0),
			backend_factory(memory_manager, sizeof(T), initial_capacity)
		{}

        template<typename... Args>
        T* createObject(Args&&... args)
        {
			return backend_factory.template createObject<T>(counter++, std::forward<Args>(args)...);
        }

        void destroyObject(T *object)
        {
			backend_factory.destroyObject(object);
        }

    private:
        size_t counter;

		Factory<T> backend_factory;
    };
}