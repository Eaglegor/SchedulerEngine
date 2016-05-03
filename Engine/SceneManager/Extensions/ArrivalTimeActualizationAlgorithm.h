#pragma once

namespace Scheduler
{
    class Schedule;
    
    class ArrivalTimeActualizationAlgorithm
    {
        public:
            virtual ~ArrivalTimeActualizationAlgorithm(){}
            
            virtual void actualize(Schedule* schedule) = 0;
    };
}