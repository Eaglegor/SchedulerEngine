#pragma once

#include "OperationStopMappingQuery.h"


namespace Scheduler
{
	/**
	 * @ingroup scene_queries
	 * 
	 * @brief Interface to perform the scene queries
	 * 
	 * @details Scene queries help to extract calculations of some information from the
	 * scene interface keeping it as small as possible. Queries can be used e.g. to find the
	 * iterators by the operations, to check the compatibility of orders and resources etc.
	 * 
	 * Some queries implementations may use caching inside, some may not - read the docs to the
	 * particular query.
	 * 
	 * Queries are initialized using the lazy calculations paradigm - so until the particular 
	 * query is requested it isn't initialized (and so no cache calculations are performed and so on).
	 */
	class SceneQueries
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param scene Scene to perform queries for
		 */
		explicit SceneQueries(Scene& scene);

		/**
		 * @brief Returns interface to find allocated stops iterators by the operations
		 * 
		 * @return Interface to find allocated stops iterators by the operations
		 */
		OperationStopMappingQuery& operationStopMapping( ) const;

	private:
		Scene& scene;

		mutable boost::optional<OperationStopMappingQuery> operation_stop_mapping_query;
	};
}