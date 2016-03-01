#pragma once

#include <type_traits>

namespace Scheduler
{
    template<typename BaseClass>
    class Factory
    {
    public:

        template<typename DerivedClass, typename... Args>
        BaseClass* createObject(Args&&...args)
        {
            static_assert(std::is_base_of<BaseClass, DerivedClass>::value, "Requested object type is incompatible with factory base type");
            return new DerivedClass(std::forward<Args>(args)...);
        }

        void destroyObject(BaseClass* object)
        {
            delete object;
        }
    };
}