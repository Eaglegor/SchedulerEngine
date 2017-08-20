#pragma once

namespace Scheduler
{
	class WorkStop;
	class DepotStop;

	class ConstStopVisitor
	{
	public:
		virtual ~ConstStopVisitor() {}

		virtual void dispatch(const WorkStop& work_stop) = 0;
		virtual void dispatch(const DepotStop& run_boundary_stop) = 0;
	};
}