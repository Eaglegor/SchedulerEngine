#pragma once

namespace Scheduler
{
	/** 
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