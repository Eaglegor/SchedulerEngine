#pragma once

#include <string>
#include <SchedulerEngine_export.h>

namespace Scheduler
{
	/** 
		Class representing a performer skill (Repair, Cleaning etc.) or vehicle attribute (Freezer, Crane etc.)
	*/
    class SCHEDULERENGINE_EXPORT Attribute
    {
    public:
        Attribute(size_t id, const char* name);

		/// Returns unique (for the single scene manager) attribute id
        size_t getId() const;

		/// Returns attribute name
        const char* getName() const;

        bool operator==(const Attribute &rhs) const;
        bool operator!=(const Attribute &rhs) const;

    private:
        size_t id;
        std::string name;
    };
}