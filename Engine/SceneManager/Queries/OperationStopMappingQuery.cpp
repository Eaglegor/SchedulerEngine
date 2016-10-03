#include "OperationStopMappingQuery.h"
#include <Engine/SceneManager/Scene.h>
#include <iterator>

namespace Scheduler
{
	OperationStopMappingQuery::OperationStopMappingQuery(Scheduler::Scene* scene):
	scene(scene),
	dirty(true)
	{
		scene->addStructuralChangesListener(this);
	}
	
	OperationStopMappingQuery::~OperationStopMappingQuery()
	{
		scene->removeStructuralChangesListener(this);
	}

	boost::optional< Run::StopsList::iterator > OperationStopMappingQuery::findStop(const Operation* operation)
	{
		if(dirty) update();
		auto iter = stops.find(operation);
		if(iter == stops.end()) return boost::none;
		return iter->second;
	}

	boost::optional< Run::WorkStopsList::iterator > OperationStopMappingQuery::findWorkStop(const Operation* operation)
	{
		if(dirty) update();
		auto iter = work_stops.find(operation);
		if(iter == work_stops.end()) return boost::none;
		return iter->second;
	}
	
	void OperationStopMappingQuery::afterWorkStopCreated(Run::WorkStopsList::const_iterator iter)
	{
		if(dirty) return;
		
		stops.emplace((*iter)->getOperation(), iter.base());
		work_stops.emplace((*iter)->getOperation(), iter);
	}
	
	void OperationStopMappingQuery::beforeWorkStopDestroyed(Run::WorkStopsList::const_iterator iter)
	{
		if(dirty) return;
		
		stops.erase((*iter)->getOperation());
		work_stops.erase((*iter)->getOperation());
	}
	
	void OperationStopMappingQuery::afterStartOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		if(dirty) return;
		
		stops.emplace(operation, iter);
	}
	
	void OperationStopMappingQuery::beforeStartOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		if(dirty) return;
		
		stops.erase(operation);
	}

	void OperationStopMappingQuery::afterEndOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		if(dirty) return;
		
		stops.emplace(operation, iter);
	}
	
	void OperationStopMappingQuery::beforeEndOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation)
	{
		if(dirty) return;
		
		stops.erase(operation);
	}

	void OperationStopMappingQuery::beforeRunDestroyed(Schedule::RunsList::const_iterator iter)
	{
		if(dirty) return;
		Run* run = *iter;
		for(const Operation* operation : run->getStartStop()->getOperations())
		{
			stops.erase(operation);
		}
		for(WorkStop* stop : run->getWorkStops())
		{
			stops.erase(stop->getOperation());
			work_stops.erase(stop->getOperation());
		}
		for(const Operation* operation : run->getEndStop()->getOperations())
		{
			stops.erase(operation);
		}
	}

	void OperationStopMappingQuery::update()
	{
		stops.clear();
		work_stops.clear();
		
		for(Schedule* schedule : scene->getSchedules())
		{
			for(Run* run : schedule->getRuns())
			{
				for(const Operation* operation : run->getStartStop()->getOperations())
				{
					stops.emplace(operation, run->getStops().begin());
				}
				for(auto iter = run->getWorkStops().begin(); iter != run->getWorkStops().end(); ++iter)
				{
					stops.emplace((*iter)->getOperation(), iter.base());
					work_stops.emplace((*iter)->getOperation(), iter);
				}
				for(const Operation* operation : run->getEndStop()->getOperations())
				{
					stops.emplace(operation, std::prev(run->getStops().end()));
				}
			}
		}
		dirty = false;
	}
}