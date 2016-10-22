#pragma once

#include "Stop.h"

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <functional>
#include <Engine/Utils/ReferenceWrapper.h>
#include <SceneManager_export.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT RunBoundaryStop : public Stop
	{
	public:
		using OperationsSet = std::unordered_set<ReferenceWrapper<const Operation> >;
		
		RunBoundaryStop(const Stop::Context& context, const Location& location, Run& run);

		void addOperation(const Operation& operation);
		void removeOperation(const Operation& operation);

		bool containsOperation(const Operation& operation) const;

		virtual const Location& getLocation() const override;

		const OperationsSet& getOperations() const;

		void acceptVisitor(StopVisitor& visitor) override;
		void acceptVisitor(ConstStopVisitor& visitor) const override;

	private:
		const Location& location;
		OperationsSet operations;
	};
}