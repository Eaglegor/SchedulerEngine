#include "Strategy.h"

namespace Scheduler
{
	Strategy::Strategy():
		tsp_solvers_factory(nullptr),
		vrp_solvers_factory(nullptr),
		root_vrp_solver(nullptr)
	{
	}

	Strategy::~Strategy()
	{
		assert(vrp_solvers_factory);
		for(VRPSolver* solver : vrp_solvers)
		{
			this->vrp_solvers_factory->destroyObject(solver);
		}

		assert(tsp_solvers_factory);
		for (TSPSolver* solver : tsp_solvers)
		{
			this->tsp_solvers_factory->destroyObject(solver);
		}

		assert(run_cost_functions_factory);
		for (RunCostFunction* function : run_cost_functions)
		{
			this->run_cost_functions_factory->destroyObject(function);
		}

		assert(schedule_cost_functions_factory);
		for (ScheduleCostFunction* function : schedule_cost_functions)
		{
			this->schedule_cost_functions_factory->destroyObject(function);
		}

		assert(scene_cost_functions_factory);
		for (SceneCostFunction* function : scene_cost_functions)
		{
			this->scene_cost_functions_factory->destroyObject(function);
		}
	}

	void Strategy::optimize(Scene* scene)
	{
		if(root_vrp_solver) root_vrp_solver->optimize(scene);
	}
	
	void Strategy::setVRPSolversFactory(Factory<VRPSolver>* factory)
	{
		this->vrp_solvers_factory = factory;
	}

	void Strategy::setTSPSolversFactory(Factory<TSPSolver>* factory)
	{
		this->tsp_solvers_factory = factory;
	}

	void Strategy::setRunCostFunctionsFactory(Factory<RunCostFunction>* factory)
	{
		this->run_cost_functions_factory = factory;
	}

	void Strategy::setScheduleCostFunctionsFactory(Factory<ScheduleCostFunction>* factory)
	{
		this->schedule_cost_functions_factory = factory;
	}

	void Strategy::setSceneCostFunctionsFactory(Factory<SceneCostFunction>* factory)
	{
		this->scene_cost_functions_factory = factory;
	}

	void Strategy::setRootVRPSolver(VRPSolver* solver)
	{
		this->root_vrp_solver = solver;
	}
}
