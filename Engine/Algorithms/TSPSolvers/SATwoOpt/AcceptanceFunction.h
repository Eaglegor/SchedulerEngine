#pragma once

#pragma once

#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
    class AcceptanceFunction
    {
    public:
        virtual ~AcceptanceFunction()
        {
        }

        virtual bool calculateAcceptance(const Cost& delta, unsigned long long iterationNumber) const = 0;

        virtual const char* getName() const = 0;
    };
}
