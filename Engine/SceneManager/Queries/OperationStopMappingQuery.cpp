#include "OperationStopMappingQuery.h"
#include "../DepotOperation.h"
#include "../WorkOperation.h"
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Utils/Iterators.h>
#include <iterator>

namespace Scheduler
{
	OperationStopMappingQuery::OperationStopMappingQuery(Scene& scene)
	    : scene(scene),
	      dirty(true)
	{
		scene.addStructuralChangesListener(*this);
	}

	OperationStopMappingQuery::~OperationStopMappingQuery( )
	{
		scene.removeStructuralChangesListener(*this);
	}

	Optional<Run::ConstStopIterator> OperationStopMappingQuery::findStop(const Operation& operation) const
	{
		if(dirty) const_cast<OperationStopMappingQuery*>(this)->update( );
		auto iter = stops.find(operation);
		if(iter == stops.end( )) return None;
		return iter->second;
	}

	Optional<Run::ConstWorkStopIterator> OperationStopMappingQuery::findWorkStop(const WorkOperation& operation) const
	{
		if(dirty) const_cast<OperationStopMappingQuery*>(this)->update( );
		auto iter = work_stops.find(operation);
		if(iter == work_stops.end( )) return None;
		return iter->second;
	}

	void OperationStopMappingQuery::afterWorkStopCreated(Run::ConstWorkStopIterator iter)
	{
		if(dirty) return;

		stops.emplace(iter->getOperation( ), iter.base( ));
		work_stops.emplace(iter->getOperation( ), iter);
	}

	void OperationStopMappingQuery::beforeWorkStopDestroyed(Run::ConstWorkStopIterator iter)
	{
		if(dirty) return;

		stops.erase(iter->getOperation( ));
		work_stops.erase(iter->getOperation( ));
	}

	void OperationStopMappingQuery::afterStartOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		if(dirty) return;

		stops.emplace(operation, iter);
	}

	void OperationStopMappingQuery::beforeStartOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		if(dirty) return;

		stops.erase(operation);
	}

	void OperationStopMappingQuery::afterEndOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		if(dirty) return;

		stops.emplace(operation, iter);
	}

	void OperationStopMappingQuery::beforeEndOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation)
	{
		if(dirty) return;

		stops.erase(operation);
	}

	void OperationStopMappingQuery::beforeRunDestroyed(Schedule::ConstRunIterator iter)
	{
		if(dirty) return;
		const Run& run = *iter;
		for(const Operation& operation : run.getStartStop( ).getOperations( ))
		{
			stops.erase(operation);
		}
		for(const WorkStop& stop : run.getWorkStops( ))
		{
			stops.erase(stop.getOperation( ));
			work_stops.erase(stop.getOperation( ));
		}
		for(const Operation& operation : run.getEndStop( ).getOperations( ))
		{
			stops.erase(operation);
		}
	}

	void OperationStopMappingQuery::update( )
	{
		stops.clear( );
		work_stops.clear( );

		for(const Schedule& schedule : scene.getSchedules( ))
		{
			for(const Run& run : schedule.getRuns( ))
			{
				for(const DepotOperation& operation : run.getStartStop( ).getOperations( ))
				{
					stops.emplace(operation, run.getStops( ).begin( ));
				}
				for(auto iter = run.getWorkStops( ).begin( ); iter != run.getWorkStops( ).end( ); ++iter)
				{
					stops.emplace(iter->getOperation( ), iter.base( ));
					work_stops.emplace(iter->getOperation( ), iter);
				}
				for(const DepotOperation& operation : run.getEndStop( ).getOperations( ))
				{
					stops.emplace(operation, std::prev(run.getStops( ).end( )));
				}
			}
		}
		dirty = false;
	}
}
