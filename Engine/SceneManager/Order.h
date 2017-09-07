#pragma once

#include <string>

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include "Constraints/Order/OrderConstraints.h"
#include "SceneObjectsFactory.h"

#include <Engine/Utils/Optional.h>
#include <Engine/Utils/String.h>
#include <boost/optional.hpp>



namespace Scheduler
{
	class Operation;
	class DepotOperation;
	class WorkOperation;
	class Attribute;
	class Location;
	class Depot;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Order that has to be completed
	 * 
	 * @details Order is a sequence of operations that have to be performed to satisfy the customer. Orders can
	 * have different types: deliveries, collections, services.
	 * 
	 * Three type of operations are associated with order:
	 *   -# start operation (optional) - base operation that has to be perfomed before starting the run
	 *   -# work operation (mandatory) - field operation that has to be performed at the customer's location
	 *   -# end operation (optional) - base operation that has to be perfomed after finishing the run
	 */
	class Order
	{
	public:
		/**
		 * @brief Internal order context
		 */
		struct Context
		{
			/**
			 * @brief Factory used to create/destroy work operation
			 */
			SceneObjectsFactory<WorkOperation>& work_operations_factory;

			/**
			 * @brief Factory used to create/destroy start and end operations
			 */
			SceneObjectsFactory<DepotOperation>& depot_operations_factory;
		};

		/**
		 * @brief Constructor
		 * 
		 * @note Work operation is automatically created by this constructor
		 * 
		 * @param id Identifier of order
		 * @param context Internal context of order
		 * @param work_operation_location Customer's location
		 */
		Order(std::size_t id, const Context& context, const Location& work_operation_location);
		~Order( );

		/**
		 * @brief Returns the identifier of order
		 * 
		 * @return Identifier of order
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns name of the order
		 * 
		 * @return Name of the order
		 */
		const String& getName( ) const;

		/**
		 * @brief Sets name of the order
		 * 
		 * @param name Name of the order
		 */
		void setName(const String& name);

		/**
		 * @brief Returns the optional start operation of the order
		 * 
		 * @return Optional start operation of the order
		 */
		Optional<const DepotOperation&> getStartOperation( ) const;

		/**
		 * @brief Returns the work operation of the order
		 * 
		 * @return Work operation of the order
		 */
		const WorkOperation& getWorkOperation( ) const;

		/**
		 * @brief Returns the work operation of the order
		 * 
		 * @return Work operation of the order
		 */
		WorkOperation& getWorkOperation( );

		/**
		 * @brief Returns the optional end operation of the order
		 * 
		 * @return Optional end operation of the order
		 */
		Optional<const DepotOperation&> getEndOperation( ) const;

		/**
		 * @brief Creates the start operation
		 * 
		 * @return Reference to the created start operation
		 */
		DepotOperation& createStartOperation( );
		
		/**
		 * @brief Creates the end operation
		 * 
		 * @return Reference to the created end operation
		 */
		DepotOperation& createEndOperation( );

		/**
		 * @brief Returns reference to the order constraints
		 * 
		 * @return Reference to the order constraints
		 */
		const OrderConstraints& constraints( ) const;

		/**
		 * @brief Returns reference to the order constraints
		 * 
		 * @return Reference to the order constraints
		 */
		OrderConstraints& constraints( );

		/**
		 * @brief Returns internal order context
		 * 
		 * @return Internal order context
		 */
		const Context& getContext( ) const;

		/**
		 * @brief Equality operator
		 * 
		 * @return true, if rhs is the same object as this, false otherwise
		 */
		bool operator==(const Order& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Order& rhs) const;

	private:
		std::size_t id;
		Context context;

		String name;

		Optional<DepotOperation&> start_operation;
		WorkOperation& work_operation;
		Optional<DepotOperation&> end_operation;

		OrderConstraints order_constraints;
	};
}
