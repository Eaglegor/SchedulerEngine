#pragma once

#include <cstdint>
#include <cstddef>

namespace Scheduler
{
    template<typename T>
    class ConceptFactory
    {
    public:
        ConceptFactory(): counter(0){}

        template<typename... Args>
        T* createObject(Args&&... args)
        {
            return new T(counter++, std::forward<Args>(args)...);
        }

        void destroyObject(T *object)
        {
            delete object;
        }

    private:
        size_t counter;
    };
}