#include <sch_test_all>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][functional][vrp_solvers]")
{
    using namespace Scheduler;

	CrowFlyRoutingService routing_service;

	EngineContext context;
	context.routing_service = &routing_service;
	Engine engine(context);

    SceneManager *sm = engine.getSceneManager();

    Scene* scene = sm->createScene();

	StrategiesManager *strategies_manager = engine.getStrategiesManager();

	Strategy* strategy = strategies_manager->createStrategy();

	TransparentVRPSolver* solver = strategy->createVRPSolver<TransparentVRPSolver>();

	solver->optimize(scene);

}