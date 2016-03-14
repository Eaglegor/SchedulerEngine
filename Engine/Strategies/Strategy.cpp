#include "Strategy.h"

namespace Scheduler
{
	Strategy::Strategy():
		tsp_solvers_factory(nullptr),
		vrp_solvers_factory(nullptr)
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
	}

	void Strategy::setVRPSolversFactory(Factory<VRPSolver>* factory)
	{
		this->vrp_solvers_factory = factory;
	}

	void Strategy::setTSPSolversFactory(Factory<TSPSolver>* factory)
	{
		this->tsp_solvers_factory = factory;
	}
}
