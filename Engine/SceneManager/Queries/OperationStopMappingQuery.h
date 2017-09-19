#pragma once

#include "../Listeners/StructuralChangesListener.h"
#include "../Run.h"
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>

#include <boost/optional.hpp>
#include <unordered_map>

namespace Scheduler
{
	class Scene;

	/**
	 * @ingroup scene_queries
	 * 
	 * @brief Query interface searching the allocated stop iterator for the specified operation
	 * 
	 * @details This query uses caching inside. The cache is initialized on the query creation and
	 * then is incrementally updated using the structural changes listeners mechanism.
	 * 
	 * @todo Make inheritance private?
	 */
	class OperationStopMappingQuery : public StructuralChangesListener
	{
	public:
		explicit OperationStopMappingQuery(Scene& scene);
		~OperationStopMappingQuery( );

		/**
		 * @brief Returns iterator to the stop holding the specified operation
		 * 
		 * @return Iterator to the stop holding the specified operation if allocated, empty Optional otherwise
		 */
		Optional<Run::ConstStopIterator> findStop(const Operation& operation) const;

		/**
		 * @brief Returns iterator to the work stop holding the specified operation
		 * 
		 * @return Iterator to the work stop holding the specified operation if allocated, empty Optional otherwise
		 */
		Optional<Run::ConstWorkStopIterator> findWorkStop(const WorkOperation& operation) const;

		virtual void afterWorkStopCreated(Run::ConstWorkStopIterator iter) override;
		virtual void beforeWorkStopDestroyed(Run::ConstWorkStopIterator iter) override;

		virtual void afterStartOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation) override;
		virtual void beforeStartOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation) override;

		virtual void afterEndOperationAdded(Run::ConstStopIterator iter, const DepotOperation& operation) override;
		virtual void beforeEndOperationRemoved(Run::ConstStopIterator iter, const DepotOperation& operation) override;

		virtual void beforeRunDestroyed(Schedule::ConstRunIterator iter) override;

	private:
		void update( );

		Scene& scene;
		bool dirty;
		mutable std::unordered_map<ReferenceWrapper<const Operation>, Run::ConstStopIterator> stops;
		mutable std::unordered_map<ReferenceWrapper<const Operation>, Run::ConstWorkStopIterator> work_stops;
	};
}
