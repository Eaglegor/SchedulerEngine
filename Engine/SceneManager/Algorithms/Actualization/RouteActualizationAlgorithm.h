#pragma once

#include <Engine/AlgorithmsManager/Algorithm.h>

namespace Scheduler
{
    class Stop;
    
    class RouteActualizationAlgorithm : public Algorithm
    {
        public:
            virtual void actualize(Stop& stop) const = 0;
    };
}