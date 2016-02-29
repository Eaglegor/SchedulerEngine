#pragma once

#include <string>

namespace Scheduler
{
    class Attribute
    {
    public:
        Attribute(size_t id, const char* name);

        size_t getId() const;

        const char* getName() const;

        bool operator==(const Attribute &rhs) const;
        bool operator!=(const Attribute &rhs) const;

    private:
        size_t id;
        std::string name;
    };
}