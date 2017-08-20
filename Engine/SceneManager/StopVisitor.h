#pragma once

namespace Scheduler
{
	class WorkStop;
	class DepotStop;

	class StopVisitor
	{
	public:
		virtual ~StopVisitor( )
		{
		}

		virtual void dispatch(WorkStop& work_stop)          = 0;
		virtual void dispatch(DepotStop& run_boundary_stop) = 0;
	};
}