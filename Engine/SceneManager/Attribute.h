#pragma once

#include <string>
#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class SCHEDULERENGINE_EXPORT Attribute
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