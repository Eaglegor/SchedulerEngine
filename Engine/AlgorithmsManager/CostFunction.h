#pragma once

namespace Scheduler
{
	class CostFunction
	{
	public:
		virtual ~CostFunction( )
		{
		}

		virtual const char* getName( ) const = 0;
	};
}