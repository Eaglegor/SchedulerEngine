#pragma once

namespace Scheduler
{
    class Stop;
    
    class RouteActualizationAlgorithm
    {
        public:
            virtual ~RouteActualizationAlgorithm(){}
            
            virtual void actualize(Stop* stop) = 0;
    };
}