#pragma once

#include "../Listeners/StructuralChangesListener.h"
#include "../Run.h"
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <SceneManager_export.h>
#include <boost/optional.hpp>
#include <unordered_map>

namespace Scheduler
{
	class Scene;

	class SCENEMANAGER_EXPORT OperationStopMappingQuery : public StructuralChangesListener
	{
	public:
		explicit OperationStopMappingQuery(Scene& scene);
		~OperationStopMappingQuery( );

		Optional<Run::ConstStopIterator> findStop(const Operation& operation) const;
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
