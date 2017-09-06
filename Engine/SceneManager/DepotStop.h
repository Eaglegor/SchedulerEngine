#pragma once

#include "Stop.h"

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/ReferenceWrapper.h>

#include <functional>

namespace Scheduler
{
	class DepotOperation;

	class DepotStop : public Stop
	{
	public:
		using OperationsSet = std::unordered_set<ReferenceWrapper<const DepotOperation>>;

		DepotStop(const Stop::Context& context, const Location& location, Run& run);

		void addOperation(const DepotOperation& operation);
		void removeOperation(const DepotOperation& operation);

		bool containsOperation(const DepotOperation& operation) const;

		virtual const Location& getLocation( ) const override;

		const OperationsSet& getOperations( ) const;

		void acceptVisitor(StopVisitor& visitor) override;
		void acceptVisitor(ConstStopVisitor& visitor) const override;

	private:
		const Location& location;
		OperationsSet operations;
	};
}
