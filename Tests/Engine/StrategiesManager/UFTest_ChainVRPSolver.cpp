#include <sch_test_all>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][functional][vrp_solvers]")
{
    using namespace Scheduler;

	CrowFlyRoutingService crow_fly_routing_service;
	
	EngineContext context;
	context.routing_service = &crow_fly_routing_service;

	Engine engine(context);

    SceneManager *sm = engine.getSceneManager();

    Scene* scene = sm->createScene();

	StrategiesManager *strategies_manager = engine.getStrategiesManager();

	Strategy* strategy = strategies_manager->createStrategy();

	TransparentVRPSolver* solver1 = strategy->createVRPSolver<TransparentVRPSolver>();
	TransparentVRPSolver* solver2 = strategy->createVRPSolver<TransparentVRPSolver>();

	ChainVRPSolver* chain_solver = strategy->createVRPSolver<ChainVRPSolver>();
	
	chain_solver->appendSolver(solver1);
	chain_solver->appendSolver(solver2);

	chain_solver->optimize(scene);

}