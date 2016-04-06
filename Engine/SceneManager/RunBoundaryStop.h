#pragma once

#include "Stop.h"

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT RunBoundaryStop : public Stop
	{
	public:
		RunBoundaryStop(const Location& location, Run* run);

		void addOperation(const Operation* operation);
		void removeOperation(const Operation* operation);

		bool containsOperation(const Operation* operation) const;

		virtual const Location& getLocation() const override;

		ImmutableUnorderedSet<const Operation*> getOperations() const;

		void acceptVisitor(StopVisitor* visitor) override;

	private:
		Location location;
		std::unordered_set<const Operation*> operations;
	};
}