#pragma once

namespace Scheduler
{
    class Run;
    
    class DurationActualizationAlgorithm
    {
        public:
            virtual ~DurationActualizationAlgorithm(){}
            
            virtual void actualize(Run* run) = 0;
    };
}