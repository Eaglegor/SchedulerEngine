#pragma once

#include "OperationStopMappingQuery.h"


namespace Scheduler
{
	class SceneQueries
	{
	public:
		explicit SceneQueries(Scene& scene);

		OperationStopMappingQuery& operationStopMapping( ) const;

	private:
		Scene& scene;

		mutable boost::optional<OperationStopMappingQuery> operation_stop_mapping_query;
	};
}