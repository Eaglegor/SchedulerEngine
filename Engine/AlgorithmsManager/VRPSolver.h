#pragma once

#include "Algorithm.h"

namespace Scheduler
{
	class Scene;

	/** 
	 * @ingroup algorithms_manager
	 * 
	 * @brief Represents abstract VRP solver interface
	 * 
	 * @details VRP (Vehicle Routing Problem) solver is an algorithm that takes the information about the orders 
	 * and vehicles fleet (scene) and transforms the allocations of orders to the vehicles
	 * 
	 * @note Usually the resulting scene is more optimal in terms of some cost function. But some algorithms may
	 * transform the solution in some predetermined way without considering any costs (i.e. randomly allocate orders 
	 * to vehicles or just unallocate all orders). This non-optimizing solvers can be used as builing blocks of 
	 * complex optimization strategies.
	 */
	class VRPSolver : public Algorithm
	{
	public:
		/**
		 * @brief Optimizes the allocations on the scene
		 * 
		 * @note Since the method is const it should be safe for the external users to call it from any context 
		 * (i.e. from different threads). State may be changed internally (i.e. for collecting stats) by the implementations 
		 * but only if such changes keep it safe to call this method from any context.
		 * 
		 * @params scene Scene to be optimized
		 */
		virtual void optimize(Scene& scene) const = 0;
	};
}