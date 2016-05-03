#include "ScheduleActualizationModel.h"

namespace Scheduler
{
        RouteActualizationAlgorithm* ScheduleActualizationModel::getRouteActualizationAlgorithm() const
        {
            return route_actualization_algorithm;
        }
        
        ArrivalTimeActualizationAlgorithm* ScheduleActualizationModel::getArrivalTimeActualizationAlgorithm() const
        {
            return arrival_time_actualization_algorithm;
        }
        
        DurationActualizationAlgorithm* ScheduleActualizationModel::getDurationActualizationAlgorithm() const
        {
            return duration_actualization_algorithm;
        }
        
        void ScheduleActualizationModel::setRouteActualizationAlgorithm(RouteActualizationAlgorithm* algorithm)
        {
            route_actualization_algorithm = algorithm;
        }
        
        void ScheduleActualizationModel::setArrivalTimeActualizationAlgorithm(ArrivalTimeActualizationAlgorithm* algorithm)
        {
            arrival_time_actualization_algorithm = algorithm;
        }
        
        void ScheduleActualizationModel::setDurationActualizationAlgorithm(DurationActualizationAlgorithm* algorithm)
        {
            duration_actualization_algorithm = algorithm;
        }
}