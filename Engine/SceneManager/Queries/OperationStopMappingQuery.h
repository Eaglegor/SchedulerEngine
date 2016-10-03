#pragma once

#include <unordered_map>
#include "../Run.h"
#include "../Listeners/StructuralChangesListener.h"
#include <boost/optional.hpp>
#include <SceneManager_export.h>

namespace Scheduler
{
	class Scene;
	
	class SCENEMANAGER_EXPORT OperationStopMappingQuery : public StructuralChangesListener
	{
	public:
		OperationStopMappingQuery(Scene* scene);
		~OperationStopMappingQuery();
		
		boost::optional<Run::StopsList::iterator> findStop(const Operation* operation);
		boost::optional<Run::WorkStopsList::iterator> findWorkStop(const Operation* operation);
		
		virtual void afterWorkStopCreated(Run::WorkStopsList::const_iterator iter);
		virtual void beforeWorkStopDestroyed(Run::WorkStopsList::const_iterator iter);
		
		virtual void afterStartOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation);
		virtual void beforeStartOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation);
		
		virtual void afterEndOperationAdded(Run::StopsList::const_iterator iter, const Operation* operation);
		virtual void beforeEndOperationRemoved(Run::StopsList::const_iterator iter, const Operation* operation);
		
		virtual void beforeRunDestroyed(Schedule::RunsList::const_iterator iter);
		
	private:
		void update();
		
		Scene* scene;
		bool dirty;
		std::unordered_map<const Operation*, Run::StopsList::iterator> stops;
		std::unordered_map<const Operation*, Run::WorkStopsList::iterator> work_stops;
	};
}