#pragma once

#include "OperationStopMappingQuery.h"
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT SceneQueries
	{
	public:
		explicit SceneQueries(Scene& scene);

		OperationStopMappingQuery& operationStopMapping( ) const;

	private:
		Scene& scene;

		mutable boost::optional<OperationStopMappingQuery> operation_stop_mapping_query;
	};
}