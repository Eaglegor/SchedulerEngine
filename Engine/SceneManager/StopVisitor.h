#pragma once

namespace Scheduler
{
	class WorkStop;
	class DepotStop;

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Represents visitor to process WorkStop and DepotStop in an unified manner
	 */
	class StopVisitor
	{
	public:
		virtual ~StopVisitor( )
		{
		}

		/**
		 * @brief Performs action on the instance of WorkStop
		 * 
		 * @param work_stop WorkStop to perform action on
		 */
		virtual void dispatch(WorkStop& work_stop)          = 0;

		/**
		 * @brief Performs action on the instance of DepotStop
		 * 
		 * @param run_boundary_stop DepotStop to perform action on
		 */
		virtual void dispatch(DepotStop& run_boundary_stop) = 0;
	};
}