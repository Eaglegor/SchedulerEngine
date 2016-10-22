#pragma once

namespace Scheduler
{
	class WorkStop;
	class RunBoundaryStop;

	class StopVisitor
	{
	public:
		virtual ~StopVisitor() {}

		virtual void dispatch(WorkStop& work_stop) = 0;
		virtual void dispatch(RunBoundaryStop& run_boundary_stop) = 0;
	};
}