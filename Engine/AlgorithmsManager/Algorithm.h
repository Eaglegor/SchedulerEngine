#pragma once

namespace Scheduler
{
	/** 
	 * @ingroup algorithms_manager
	 * 
	 * @brief Represents the abstract algorithm interface
	 */
	class Algorithm
	{
	public:
		virtual ~Algorithm( )
		{
		}

		/** 
		 * @brief Gets the algorithm name
		 * @return Algorithm name
		 */
		virtual const char* getName( ) const = 0;
	};
}