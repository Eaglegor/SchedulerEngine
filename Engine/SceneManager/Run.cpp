#include <assert.h>
#include "Run.h"
#include "Stop.h"
#include "Vehicle.h"
#include "ScheduleActualizationModel.h"
#include "ScheduleValidationModel.h"
#include "Algorithms/Validation/ValidationAlgorithm.h"
#include "Algorithms/Validation/Validator.h"
#include "WorkStop.h"
#include "Schedule.h"
#include <algorithm>
#include <Engine/Utils/AutoCastRange.h>
#include "Listeners/StructuralChangesObserver.h"


namespace Scheduler {

    Run::Run(std::size_t id, const Context& context, Schedule &schedule, const Location &start_location, const Location &end_location, Schedule::StopsList &stops_list, Schedule::StopsList::const_iterator pos):
	id(id),
	context(context),
	schedule(schedule),
	start_stop(Stop::Context{duration_actualizer, context.arrival_time_actualizer}, start_location, *this),
	end_stop(Stop::Context{duration_actualizer, context.arrival_time_actualizer}, end_location, *this),
	duration_actualizer(*this),
	schedule_stops(stops_list),
	is_detached(false)
    {
		stops = std::move(StopsRange(&stops_list, pos.unconst(), pos.unconst()));

		stops.insert(stops.end(), start_stop);
		stops.insert(stops.end(), end_stop);
		
		work_stops = std::move(WorkStopsRange(&stops, std::prev(stops.end()), std::prev(stops.end())));
		casted_work_stops = WorkStopsList(&work_stops);
		
		assert(isConsistent());
    }

    std::size_t Run::getId() const 
    {
        return id;
    }

    const Schedule& Run::getSchedule() const 
    {
        return schedule;
    }
    
	Schedule& Run::getSchedule()
	{
		return schedule;
	}

    Optional<const Vehicle&> Run::getVehicle() const 
    {
        return vehicle;
    }

    const RunBoundaryStop& Run::getStartStop() const 
    {
        return start_stop;
    }

    RunBoundaryStop& Run::getStartStop() 
	{
        return start_stop;
    }

	const Run::WorkStopsList& Run::getWorkStops() const
	{
		return casted_work_stops;
	}
	
	Run::WorkStopsList& Run::getWorkStops()
	{
		return casted_work_stops;
	}

	const Run::StopsList& Run::getStops() const
	{
		return stops;
	}
	
	Run::StopsList& Run::getStops()
	{
		return stops;
	}
	
    const RunBoundaryStop& Run::getEndStop() const 
    {
        return end_stop;
    }

	RunBoundaryStop& Run::getEndStop() 
	{
        return end_stop;
    }

    void Run::allocateStartOperation(const Operation &operation) 
	{
		assert(!is_detached);
		
        start_stop.addOperation(operation);
		
		context.structural_changes_observer.afterStartOperationAdded(stops.begin(), operation);
    }
    
    Run::WorkStopIterator Run::createWorkStop(ConstWorkStopIterator pos, const Operation &operation) 
	{
		assert(!is_detached);
		
		WorkStop &stop = *context.stops_factory.createObject(Stop::Context{duration_actualizer, context.arrival_time_actualizer}, *this, operation);
		
		auto iter = casted_work_stops.insert(pos, stop);

		context.arrival_time_actualizer.setDirty(true);
		duration_actualizer.setDirty(true);

		context.structural_changes_observer.afterWorkStopCreated(iter);
		
		assert(isConsistent());
		
        return iter;
    }

    void Run::allocateEndOperation(const Operation &operation) 
	{
		assert(!is_detached);
		
        end_stop.addOperation(operation);
		
		context.structural_changes_observer.afterEndOperationAdded(std::prev(stops.end()), operation);
    }

    void Run::unallocateStartOperation(const Operation &operation) 
	{
		assert(!is_detached);
		
		context.structural_changes_observer.beforeStartOperationRemoved(stops.begin(), operation);
		
        start_stop.removeOperation(operation);
    }

    Run::WorkStopsList::iterator Run::destroyWorkStop(WorkStopsList::const_iterator pos) 
	{
		assert(!is_detached);
		
		context.structural_changes_observer.beforeWorkStopDestroyed(pos);
		
		auto iter = casted_work_stops.erase(pos);
		
        context.stops_factory.destroyObject(const_cast<WorkStop*>(&*pos));
		
		context.arrival_time_actualizer.setDirty(true);
		duration_actualizer.setDirty(true);
		
		assert(isConsistent());
		
		return iter;
    }

    void Run::unallocateEndOperation(const Operation &operation) 
	{
		assert(!is_detached);
		
		context.structural_changes_observer.beforeEndOperationRemoved(std::prev(stops.end()), operation);
		
        end_stop.removeOperation(operation);
    }

	bool Run::isValid() const
	{
		assert(!is_detached);
		
		Validator validator;
		validate(validator);
		return validator.getValidationResult() == Validator::ValidationResult::VALID;
	}

	void Run::validate(ViolationsConsumer& violations_consumer) const
	{
		assert(!is_detached);
		
		schedule.getValidationModel().getRunValidationAlgorithm().validate(*this, violations_consumer);
	}
	
    void Run::setVehicle(Optional<const Vehicle&> vehicle) {
		assert(!is_detached);
		
		if(!vehicle && this->vehicle) context.arrival_time_actualizer.setDirty(true);
		else if(vehicle && !this->vehicle) context.arrival_time_actualizer.setDirty(true);
		else if(vehicle && this->vehicle && vehicle->getRoutingProfile() != this->vehicle->getRoutingProfile()) context.arrival_time_actualizer.setDirty(true);
        
		this->vehicle = vehicle;
    }

    Run::~Run() {
		if(!isDetached()) detach();
		
		detached_stops.pop_front();
		detached_stops.pop_back();
		
		detached_stops.clear_and_dispose([&](Stop* s){context.stops_factory.destroyObject(static_cast<WorkStop*>(s));});
    }
	
	void Run::swapWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator second)
	{
		assert(!is_detached);
		
		if(first == second) return;

		context.structural_changes_observer.beforeWorkStopsSwapped(first, second);
		
		if(std::next(first) == second)
		{
			casted_work_stops.splice(first, casted_work_stops, second);
			return;
		} 
		else if(std::next(second) == first)
		{
			casted_work_stops.splice(second, casted_work_stops, first);
			return;
		}
		ConstWorkStopIterator pos = std::next(second);
		casted_work_stops.splice(first, casted_work_stops, second);
		casted_work_stops.splice(pos, casted_work_stops, first);
		
		context.arrival_time_actualizer.setDirty(true);
		duration_actualizer.setDirty(true);
		
		assert(isConsistent());
	}

	void Run::reverseWorkStops()
	{
		reverseWorkStops(casted_work_stops.begin(), casted_work_stops.end());
	}

	void Run::reverseWorkStops(ConstWorkStopIterator first, ConstWorkStopIterator last)
	{
		assert(!is_detached);
		
		context.structural_changes_observer.beforeWorkStopsReversed(first, last);
		
		casted_work_stops.reverse(first, last);
		
		context.arrival_time_actualizer.setDirty(true);
		duration_actualizer.setDirty(true);
		
		assert(isConsistent());
	}

	void Run::spliceWorkStops(ConstWorkStopIterator pos, ConstWorkStopIterator first, ConstWorkStopIterator last)
	{
		spliceWorkStops(pos, *this, first, last);
	}

	void Run::spliceWorkStops(ConstWorkStopIterator pos, Run& from, ConstWorkStopIterator first, ConstWorkStopIterator last, Optional<std::size_t> n)
	{
		assert(!is_detached);
		
		context.structural_changes_observer.beforeWorkStopsSpliced(*this, pos, from, first, last);

		if(n) casted_work_stops.splice(pos, from.casted_work_stops, first, last, n.get());
		else casted_work_stops.splice(pos, from.casted_work_stops, first, last);
		
		if(this != &from)
		{
			for(auto iter = first.base().unconst(); iter != pos.base(); ++iter)
			{
				iter->run = *this;
			}
		}
		
		context.arrival_time_actualizer.setDirty(true);
		duration_actualizer.setDirty(true);
		
		assert(isConsistent());
	}
	
	void Run::setStopsEndIterator(ConstStopIterator end)
	{
		assert(!is_detached);
		
		stops.setEnd(end, stops.size());
		
		assert(isConsistent());
	}
	
	bool Run::operator==(const Run& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Run::operator!=(const Run& rhs) const
	{
		return !(*this == rhs);
	}
	
	Run::ConstStopIterator Run::findStop(const Stop& stop) const
	{
		return Schedule::StopsList::s_iterator_to(stop);
	}

	Run::StopIterator Run::findStop(Stop& stop)
	{
		return Schedule::StopsList::s_iterator_to(stop);
	}

	Run::ConstWorkStopIterator Run::findWorkStop(const WorkStop& stop) const
	{
		return WorkStopsList::const_iterator(Schedule::StopsList::s_iterator_to(stop));
	}

	Run::WorkStopIterator Run::findWorkStop(WorkStop& stop)
	{
		return WorkStopsList::iterator(Schedule::StopsList::s_iterator_to(stop));
	}

	bool Run::isConsistent() const
	{
		std::size_t counter = 0;
		for(auto iter = stops.begin(); iter != stops.end(); ++iter)
		{
			if(&*iter == nullptr) 
			{
				return false;
			}
			if(counter >= stops.size()) 
			{
				return false;
			}
			++counter;
		}
		if(counter != stops.size()) 
		{
			return false;
		}
		
		counter = 0;
		for(auto iter = work_stops.begin(); iter != work_stops.end(); ++iter)
		{
			if(&*iter == nullptr) 
			{
				return false;
			}
			if(counter >= work_stops.size()) 
			{
				return false;
			}
			++counter;
		}
		if(counter != work_stops.size()) 
		{
			return false;
		}
		
		counter = 0;
		for(auto iter = casted_work_stops.begin(); iter != casted_work_stops.end(); ++iter)
		{
			if(&*iter == nullptr) 
			{
				return false;
			}
			if(counter >= casted_work_stops.size()) 
			{
				return false;
			}
			++counter;
		}
		if(counter != casted_work_stops.size()) 
		{
			return false;
		}
		
		return true;
	}
	
	void Run::detach()
	{
		assert(!is_detached);
		is_detached = true;
		detached_stops.splice(detached_stops.end(), schedule_stops, stops.begin(), stops.end(), stops.size());
	}

	void Run::attach(Schedule::StopsList::const_iterator pos)
	{
		assert(is_detached);

		schedule_stops.splice(pos, detached_stops, detached_stops.begin(), detached_stops.end(), detached_stops.size());
		stops.setEnd(pos);
		//work_stops.setEnd(std::prev(stops.end()));
	
		is_detached = false;
		assert(isConsistent());
	}

	bool Run::isDetached() const
	{
		return is_detached;
	}

}