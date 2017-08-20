#include "Stop.h"
#include "Algorithms/Validation/ValidationAlgorithm.h"
#include "Algorithms/Validation/Validator.h"
#include "ArrivalTimeActualizer.h"
#include "DurationActualizer.h"
#include "Location.h"
#include "RouteActualizer.h"
#include "Run.h"
#include "ScheduleActualizationModel.h"
#include "ScheduleValidationModel.h"
#include <Engine/Utils/Collections/Algorithms.h>
#include <assert.h>

namespace Scheduler
{

	Stop::Stop(const Context& context, Run& run)
	    : context(context),
	      run(run),
	      route_actualizer(*this),
	      allocation_time(context.arrival_time_actualizer),
	      duration(context.duration_actualizer),
	      next_route(route_actualizer),
	      list_node(this)
	{
	}

	const TimeWindow& Stop::getAllocationTime( ) const
	{
		return allocation_time.get( );
	}

	void Stop::setAllocationTime(const TimeWindow& time)
	{
		this->allocation_time = time;
	}

	const Duration& Stop::getDuration( ) const
	{
		return duration.get( );
	}

	void Stop::setDuration(const Duration& duration)
	{
		this->duration = duration;
	}

	void Stop::setNextRoute(const Route& route)
	{
		this->next_route = route;
	}

	const Run& Stop::getRun( ) const
	{
		return run;
	}

	Run& Stop::getRun( )
	{
		return run;
	}

	const Route& Stop::getNextRoute( ) const
	{
		return next_route.get( );
	}

	bool Stop::isValid( ) const
	{
		Validator validator;
		validate(validator);
		return validator.getValidationResult( ) == Validator::ValidationResult::VALID;
	}

	void Stop::validate(ViolationsConsumer& violations_consumer) const
	{
		run.get( ).getSchedule( ).getValidationModel( ).getStopValidationAlgorithm( ).validate(*this, violations_consumer);
	}

	void Stop::setStartTime(const TimePoint& time)
	{
		this->setAllocationTime(TimeWindow(time, time + duration.get( )));
	}

	void Stop::setEndTime(const TimePoint& time)
	{
		this->setAllocationTime(TimeWindow(time - duration.get( ), time));
	}
}
