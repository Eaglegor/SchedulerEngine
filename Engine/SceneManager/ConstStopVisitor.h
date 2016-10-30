#pragma once

namespace Scheduler
{
	class WorkStop;
	class RunBoundaryStop;

	class ConstStopVisitor
	{
	public:
		virtual ~ConstStopVisitor() {}

		virtual void dispatch(const WorkStop& work_stop) = 0;
		virtual void dispatch(const RunBoundaryStop& run_boundary_stop) = 0;
	};
}