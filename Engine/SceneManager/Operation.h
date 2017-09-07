#pragma once

#include "Constraints/Operation/OperationConstraints.h"
#include "Order.h"
#include <Engine/Concepts/Capacity.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/Concepts/Site.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Optional.h>
#include <boost/optional.hpp>
#include <string>



namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @param Operation that has to be performed to complete the order
	 * 
	 * @details Operation is something that has to be done by the performer to complete the order. This can be
	 * delivering thing to the customer, collecting a package, servicing customer etc.
	 */
	class Operation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of operation
		 * @param order Order the operation belongs to
		 */
		Operation(std::size_t id, const Order& order);
		virtual ~Operation( ){}

		/**
		 * @brief Returns name of the operation
		 * 
		 * @return Name of the operation
		 */
		const String& getName( ) const;

		/**
		 * @brief Returns identifier of the operation
		 * 
		 * @return Identifier of the operation
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns location of the operation
		 * 
		 * @return Location of the operation
		 */
		const Location& getLocation( ) const;

		/**
		 * @brief Returns duration of the operation
		 * 
		 * @return Duration of the operation
		 */
		const Duration& getDuration( ) const;

		/**
		 * @brief Returns the order the operation belongs to
		 * 
		 * @return Order the operation belongs to
		 */
		const Order& getOrder( ) const;

		/**
		 * @brief Sets name of the operation
		 * 
		 * @param name Name of the operation
		 */
		void setName(const String& name);

		/**
		 * @brief Sets duration of the operation
		 * 
		 * @param duration Duration of the operation
		 */
		void setDuration(const Duration& duration);

		/**
		 * @brief Returns reference to the operation constraints
		 * 
		 * @return Reference to the operation constraints
		 */
		const OperationConstraints& constraints( ) const;

		/**
		 * @brief Returns reference to the operation constraints
		 * 
		 * @return Reference to the operation constraints
		 */
		OperationConstraints& constraints( );

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another operation
		 * 
		 * @return true, if rhs is the same object as this
		 */
		bool operator==(const Operation& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs Another operation
		 * 
		 * @return result of !(rhs == *this)
		 */
		bool operator!=(const Operation& rhs) const;

	private:
		std::size_t id;
		String name;

		Duration duration;

		const Order& order;

		OperationConstraints operation_constraints;
	};
}
