#pragma once

namespace Scheduler
{
	class Algorithm
	{
	public:
		virtual ~Algorithm( )
		{
		}

		virtual const char* getName( ) const = 0;
	};
}