#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
    class Run;
    
    class DurationActualizationAlgorithm : public Algorithm
    {
        public:
            virtual void actualize(Run& run) const = 0;
    };
}