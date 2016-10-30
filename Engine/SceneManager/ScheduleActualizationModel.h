#pragma once

#include <SceneManager_export.h>
#include <Engine/Utils/ReferenceWrapper.h>

namespace Scheduler
{
    class RouteActualizationAlgorithm;
    class ArrivalTimeActualizationAlgorithm;
    class DurationActualizationAlgorithm;
    
    class SCENEMANAGER_EXPORT ScheduleActualizationModel
    {
        public:
			ScheduleActualizationModel();

            const RouteActualizationAlgorithm& getRouteActualizationAlgorithm() const;
            const ArrivalTimeActualizationAlgorithm& getArrivalTimeActualizationAlgorithm() const;
            const DurationActualizationAlgorithm& getDurationActualizationAlgorithm() const;
            
            void setRouteActualizationAlgorithm(const RouteActualizationAlgorithm& algorithm);
            void setArrivalTimeActualizationAlgorithm(const ArrivalTimeActualizationAlgorithm& algorithm);
            void setDurationActualizationAlgorithm(const DurationActualizationAlgorithm& algorithm);
            
        private:
			ReferenceWrapper<const RouteActualizationAlgorithm> route_actualization_algorithm;
			ReferenceWrapper<const ArrivalTimeActualizationAlgorithm> arrival_time_actualization_algorithm;
			ReferenceWrapper<const DurationActualizationAlgorithm> duration_actualization_algorithm;
    };
}