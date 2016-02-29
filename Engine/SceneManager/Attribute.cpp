#include "Attribute.h"

namespace Scheduler
{
    Attribute::Attribute(size_t id, const char *name):
            id(id),
            name(name)
    {
    }

    size_t Attribute::getId() const{
        return id;
    }

    const char* Attribute::getName() const {
        return name.c_str();
    }

    bool Attribute::operator==(const Attribute &rhs) const {
        return this->id == rhs.id;
    }

    bool Attribute::operator!=(const Attribute &rhs) const {
        return !(*this == rhs);
    }
}

