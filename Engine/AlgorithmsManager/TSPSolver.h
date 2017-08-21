#pragma once

#include "Algorithm.h"

namespace Scheduler
{
	class Run;
	class Schedule;

	/** 
	 * @brief Represents abstract TSP solver interface
	 * 
	 * @details {TSP (Travelling Salesman Problem) solver is an algorithm that takes the single run or the entire schedule and transforms
	 * the sequence of orders in it to improve solution}
	 * 
	 * @note {Usually the resulting sequence is more optimal in terms of some cost function. But some algorithms may
	 * transform the solution in some predetermined way without considering any costs (i.e. reverse the sequence or build 
	 * greedy nearest neighbour solution). This non-optimizing solvers can be used as builing blocks of complex optimization
	 * strategies.}
	 */
	class TSPSolver : public Algorithm
	{
	public:
		/** 
		 * @brief Optimizes the sequence of orders in the entire schedule
		 * 
		 * @note {Since the method is const it should be safe for the external users to call it from any context (i.e. from different threads).
		 * State may be changed internally (i.e. for collecting stats) by the implementations but only if such changes keep it safe to call
		 * this method from any context.}
		 * 
		 * @param schedule Schedule to be optimized
		 */
		virtual void optimize(Schedule& schedule) const = 0;

		/**
		 * @brief Optimizes the sequence of orders in the single run of the schedule
		 * 
		 * @note {Since the method is const it should be safe for the external users to call it from any context (i.e. from different threads).
		 * State may be changed internally (i.e. for collecting stats) by the implementations but only if such changes keep it safe to call
		 * this method from any context.}
		 * 
		 * @param run Run to be optimized
		 */
		virtual void optimize(Run& run) const           = 0;
	};
}