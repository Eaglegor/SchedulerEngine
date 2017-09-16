#pragma once

#include "Operation.h"

namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Operation performed at customer's location
	 */
	class WorkOperation : public Operation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of operation
		 * @param location Location where the operation has to be performed
		 * @param order Order the operation belongs to
		 */
		WorkOperation(std::size_t id, const Location& location, const Order& order);

		/**
		 * @brief Returns location where the operation has to be performed
		 * 
		 * @return Location where the operation has to be performed
		 */
		const Location& getLocation( ) const;

	private:
		const Location& location;
	};
}
