#pragma once

#include "Stop.h"

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/ReferenceWrapper.h>

#include <functional>

namespace Scheduler
{
	class DepotOperation;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Represents being in depot in the beginning or in the end of vehicle's run
	 */
	class DepotStop : public Stop
	{
	public:
		/**
		 * @brief Represents type capable to store the set of operations
		 */
		using OperationsSet = std::unordered_set<ReferenceWrapper<const DepotOperation>>;

		/**
		 * @brief Constructor
		 * 
		 * @param context Internal context of stop
		 * @param location Location of stop
		 * @param run Vehicle's run the stop belongs to
		 */
		DepotStop(const Stop::Context& context, const Location& location, Run& run);

		/**
		 * @brief Adds an operation to the stop
		 * 
		 * @param operation Operation to add to the stop
		 */
		void addOperation(const DepotOperation& operation);

		/**
		 * @brief Removes an operation from the stop
		 * 
		 * @param operation Operation to remove from the stop
		 */
		void removeOperation(const DepotOperation& operation);

		/**
		 * @brief Checks if the stop contains the specified operation
		 * 
		 * @param operation Operation to look for in the stop
		 * 
		 * @return true, if stop contains operation, false otherwise
		 */
		bool containsOperation(const DepotOperation& operation) const;

		/**
		 * @brief Returns location of the stop
		 * 
		 * @return Location of the stop
		 */
		virtual const Location& getLocation( ) const override;

		/**
		 * @brief Returns operations belonging to this stop
		 * 
		 * @return Set of operations belonging to this stop
		 */
		const OperationsSet& getOperations( ) const;

		/**
		 * @brief Accepts visitor
		 * 
		 * @param visitor Visitor to accept
		 */
		void acceptVisitor(StopVisitor& visitor) override;

		/**
		 * @brief Accepts visitor
		 * 
		 * @param visitor Visitor to accept
		 */
		void acceptVisitor(ConstStopVisitor& visitor) const override;

	private:
		const Location& location;
		OperationsSet operations;
	};
}
