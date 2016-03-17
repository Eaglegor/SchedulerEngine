#pragma once

#include <unordered_set>
#include <Engine/Utils/Factory.h>
#include <utility>
#include "TSPSolver.h"
#include "VRPSolver.h"
#include <assert.h>

#include <StrategiesManager_export.h>
#include "CostFunctions/ScheduleCostFunction.h"
#include "CostFunctions/SceneCostFunction.h"
#include "CostFunctions/RunCostFunction.h"

namespace Scheduler
{

	class STRATEGIESMANAGER_EXPORT Strategy
	{
	public:
		Strategy();
		~Strategy();

		void optimize(Scene* scene);
		
		template<typename T, typename... Args>
		T* createVRPSolver(Args&&...args)
		{
			return createImpl<T>(vrp_solvers_factory, vrp_solvers, std::forward<Args>(args)...);
		}

		void destroyVRPSolver(VRPSolver* solver)
		{
			destroyImpl(vrp_solvers_factory, vrp_solvers, solver);
		}

		template<typename T, typename... Args>
		T* createTSPSolver(Args&&...args)
		{
			return createImpl<T>(tsp_solvers_factory, tsp_solvers, std::forward<Args>(args)...);
		}

		void destroyTSPSolver(TSPSolver* solver)
		{
			destroyImpl(tsp_solvers_factory, tsp_solvers, solver);
		}

		template<typename T, typename... Args>
		T* createRunCostFunction(Args&&...args)
		{
			return createImpl<T>(run_cost_functions_factory, run_cost_functions, std::forward<Args>(args)...);
		}

		void destroyRunCostFunction(RunCostFunction* function)
		{
			destroyImpl(run_cost_functions_factory, run_cost_functions, function);
		}

		template<typename T, typename... Args>
		T* createScheduleCostFunction(Args&&...args)
		{
			return createImpl<T>(schedule_cost_functions_factory, schedule_cost_functions, std::forward<Args>(args)...);
		}

		void destroyScheduleCostFunction(ScheduleCostFunction* function)
		{
			destroyImpl(schedule_cost_functions_factory, schedule_cost_functions, function);
		}

		template<typename T, typename... Args>
		T* createSceneCostFunction(Args&&...args)
		{
			return createImpl<T>(scene_cost_functions_factory, scene_cost_functions, std::forward<Args>(args)...);
		}

		void destroySceneCostFunction(SceneCostFunction* function)
		{
			destroyImpl(scene_cost_functions_factory, scene_cost_functions, function);
		}

		void setRootVRPSolver(VRPSolver* solver);
		
		// ===== framework internal
		void setVRPSolversFactory(Factory<VRPSolver>* factory);
		void setTSPSolversFactory(Factory<TSPSolver>* factory);

		void setRunCostFunctionsFactory(Factory<RunCostFunction>* factory);
		void setScheduleCostFunctionsFactory(Factory<ScheduleCostFunction>* factory);
		void setSceneCostFunctionsFactory(Factory<SceneCostFunction>* factory);

	private:
		template<typename T, typename Factory, typename Container, typename... Args>
		T* createImpl(Factory *factory, Container& container, Args&&...args)
		{
			assert(factory);

			T* object = factory->template createObject<T>(std::forward<Args>(args)...);

			if (object) container.emplace(object);

			return object;
		}

		template<typename T, typename Factory, typename Container>
		void destroyImpl(Factory *factory, Container& container, T* object)
		{
			assert(factory);

			container.erase(object);
			factory->destroyObject(object);
		}

		VRPSolver* root_vrp_solver;
	
		std::unordered_set<VRPSolver*> vrp_solvers;
		std::unordered_set<TSPSolver*> tsp_solvers;

		std::unordered_set<RunCostFunction*> run_cost_functions;
		std::unordered_set<ScheduleCostFunction*> schedule_cost_functions;
		std::unordered_set<SceneCostFunction*> scene_cost_functions;

		Factory<VRPSolver> *vrp_solvers_factory;
		Factory<TSPSolver> *tsp_solvers_factory;

		Factory<RunCostFunction> *run_cost_functions_factory;
		Factory<ScheduleCostFunction> *schedule_cost_functions_factory;
		Factory<SceneCostFunction> *scene_cost_functions_factory;
	};
}
