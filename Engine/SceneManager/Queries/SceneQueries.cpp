#include "SceneQueries.h"

namespace Scheduler
{
	SceneQueries::SceneQueries(Scene* scene):
	scene(scene)
	{}
	
	OperationStopMappingQuery& SceneQueries::operationStopMapping()
	{
		if(!operation_stop_mapping_query) operation_stop_mapping_query.emplace(scene);
		return operation_stop_mapping_query.get();
	}
}