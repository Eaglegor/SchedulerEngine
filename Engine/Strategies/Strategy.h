#pragma once

#include <unordered_set>
#include <Engine/SceneManager/Factory.h>
#include <utility>
#include <SchedulerEngine_export.h>
#include "TSPSolver.h"
#include "VRPSolver.h"
#include <assert.h>

namespace Scheduler
{

	class SCHEDULERENGINE_EXPORT Strategy
	{
	public:
		Strategy();
		~Strategy();

		template<typename T, typename... Args>
		T* createVRPSolver(Args&&...args)
		{
			assert(vrp_solvers_factory);

			T* vrp_solver = vrp_solvers_factory->createObject<T>(std::forward<Args>(args)...);

			if (vrp_solver) vrp_solvers.emplace(vrp_solver);

			return vrp_solver;
		}

		void destroyVRPSolver(VRPSolver* solver)
		{
			assert(vrp_solvers_factory);

			vrp_solvers.erase(solver);
			vrp_solvers_factory->destroyObject(solver);
		}

		template<typename T, typename... Args>
		T* createTSPSolver(Args&&...args)
		{
			assert(tsp_solvers_factory);

			T* tsp_solver = tsp_solvers_factory->createObject<T>(std::forward<Args>(args)...);

			if (tsp_solver) tsp_solvers.emplace(tsp_solver);

			return tsp_solver;
		}

		void destroyTSPSolver(TSPSolver* solver)
		{
			assert(tsp_solvers_factory);

			tsp_solvers.erase(solver);
			tsp_solvers_factory->destroyObject(solver);
		}

		// ===== framework internal
		void setVRPSolversFactory(Factory<VRPSolver>* factory);
		void setTSPSolversFactory(Factory<TSPSolver>* factory);

	private:
		std::unordered_set<VRPSolver*> vrp_solvers;
		std::unordered_set<TSPSolver*> tsp_solvers;

		Factory<VRPSolver> *vrp_solvers_factory;
		Factory<TSPSolver> *tsp_solvers_factory;
	};
}