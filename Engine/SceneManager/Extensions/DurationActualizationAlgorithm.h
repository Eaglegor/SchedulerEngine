#pragma once

namespace Scheduler
{
    class Stop;
    
    class DurationActualizationAlgorithm
    {
        public:
            virtual ~DurationActualizationAlgorithm(){}
            
            virtual void actualize(Stop* stop) = 0;
    };
}