#pragma once

namespace Scheduler
{
	/** 
	 * @ingroup algorithms_manager
	 * 
	 * @brief Represents abstract cost function interface
	 * 
	 * @details Cost function is a function that takes a part of solution or the entire solution and returns
	 * a value of Cost type
	 */
	class CostFunction
	{
	public:
		virtual ~CostFunction( )
		{
		}

		/** 
		 * @brief Returns the name of the cost function
		 * @return Cost function name
		 */
		virtual const char* getName( ) const = 0;
	};
}