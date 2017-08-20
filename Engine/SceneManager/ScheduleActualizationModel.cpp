#include "ScheduleActualizationModel.h"

#include "Algorithms/Actualization/ArrivalTimeActualizationAlgorithm.h"
#include "Algorithms/Actualization/DurationActualizationAlgorithm.h"
#include "Algorithms/Actualization/RouteActualizationAlgorithm.h"

namespace Scheduler
{

	class EmptyRouteActualizationAlgorithm : public RouteActualizationAlgorithm
	{
	public:
		virtual void actualize(Stop& stop) const override
		{
		}
		virtual const char* getName( ) const override
		{
			return "Empty";
		}
	};

	class EmptyDurationActualizationAlgorithm : public DurationActualizationAlgorithm
	{
	public:
		virtual void actualize(Run& stop) const override
		{
		}
		virtual const char* getName( ) const override
		{
			return "Empty";
		}
	};

	class EmptyArrivalTimeActualizationAlgorithm : public ArrivalTimeActualizationAlgorithm
	{
	public:
		virtual void actualize(Schedule& stop) const override
		{
		}
		virtual const char* getName( ) const override
		{
			return "Empty";
		}
	};

	static EmptyRouteActualizationAlgorithm EMPTY_ROUTE_ACTUALIZATION_ALGORITHM;
	static EmptyDurationActualizationAlgorithm EMPTY_DURATION_ACTUALIZATION_ALGORITHM;
	static EmptyArrivalTimeActualizationAlgorithm EMPTY_ARRIVAL_TIME_ACTUALIZATION_ALGORITHM;

	ScheduleActualizationModel::ScheduleActualizationModel( )
	    : route_actualization_algorithm(EMPTY_ROUTE_ACTUALIZATION_ALGORITHM),
	      duration_actualization_algorithm(EMPTY_DURATION_ACTUALIZATION_ALGORITHM),
	      arrival_time_actualization_algorithm(EMPTY_ARRIVAL_TIME_ACTUALIZATION_ALGORITHM)
	{
	}

	const RouteActualizationAlgorithm& ScheduleActualizationModel::getRouteActualizationAlgorithm( ) const
	{
		return route_actualization_algorithm;
	}

	const ArrivalTimeActualizationAlgorithm& ScheduleActualizationModel::getArrivalTimeActualizationAlgorithm( ) const
	{
		return arrival_time_actualization_algorithm;
	}

	const DurationActualizationAlgorithm& ScheduleActualizationModel::getDurationActualizationAlgorithm( ) const
	{
		return duration_actualization_algorithm;
	}

	void ScheduleActualizationModel::setRouteActualizationAlgorithm(const RouteActualizationAlgorithm& algorithm)
	{
		route_actualization_algorithm = algorithm;
	}

	void ScheduleActualizationModel::setArrivalTimeActualizationAlgorithm(const ArrivalTimeActualizationAlgorithm& algorithm)
	{
		arrival_time_actualization_algorithm = algorithm;
	}

	void ScheduleActualizationModel::setDurationActualizationAlgorithm(const DurationActualizationAlgorithm& algorithm)
	{
		duration_actualization_algorithm = algorithm;
	}
}