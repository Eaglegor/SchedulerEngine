#pragma once

#include "Operation.h"



namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Operation that has to be performed in depot
	 * 
	 * @details Depot operations are performed before the vehicle left it's depot or after the vehicle has returned to it's depot
	 */
	class DepotOperation : public Operation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of operation
		 * @param order Order to which the operation belongs
		 */
		DepotOperation(std::size_t id, const Order& order);
	};
}
