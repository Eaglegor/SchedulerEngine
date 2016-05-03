#pragma once

#include <SceneManager_export.h>

namespace Scheduler
{
    class RouteActualizationAlgorithm;
    class ArrivalTimeActualizationAlgorithm;
    class DurationActualizationAlgorithm;
    
    class SCENEMANAGER_EXPORT ScheduleActualizationModel
    {
        public:
			ScheduleActualizationModel():
				route_actualization_algorithm(nullptr),
				arrival_time_actualization_algorithm(nullptr),
				duration_actualization_algorithm(nullptr)
			{}

            RouteActualizationAlgorithm* getRouteActualizationAlgorithm() const;
            ArrivalTimeActualizationAlgorithm* getArrivalTimeActualizationAlgorithm() const;
            DurationActualizationAlgorithm* getDurationActualizationAlgorithm() const;
            
            void setRouteActualizationAlgorithm(RouteActualizationAlgorithm* algorithm);
            void setArrivalTimeActualizationAlgorithm(ArrivalTimeActualizationAlgorithm* algorithm);
            void setDurationActualizationAlgorithm(DurationActualizationAlgorithm* algorithm);
            
        private:
            RouteActualizationAlgorithm* route_actualization_algorithm;
            ArrivalTimeActualizationAlgorithm* arrival_time_actualization_algorithm;
            DurationActualizationAlgorithm* duration_actualization_algorithm;
    };
}