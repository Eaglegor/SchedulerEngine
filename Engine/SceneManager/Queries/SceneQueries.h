#pragma once

#include "OperationStopMappingQuery.h"
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT SceneQueries
	{
	public:
		SceneQueries(Scene* scene);
		
		OperationStopMappingQuery& operationStopMapping();
		
	private:
		Scene* scene;
		
		boost::optional<OperationStopMappingQuery> operation_stop_mapping_query;
	};
}