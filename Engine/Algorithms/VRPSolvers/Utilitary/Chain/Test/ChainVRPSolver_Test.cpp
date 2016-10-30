#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Chain/ChainVRPSolver.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][functional][vrp_solvers]")
{
    using namespace Scheduler;

	CrowFlyRoutingService crow_fly_routing_service;
	
	Engine engine;

    SceneManager& sm = engine.getSceneManager();

    Scene& scene = sm.createScene(sm.createSceneContext());

	AlgorithmsManager& am = engine.getAlgorithmsManager();

	TransparentVRPSolver& solver1 = am.createAlgorithm<TransparentVRPSolver>();
	TransparentVRPSolver& solver2 = am.createAlgorithm<TransparentVRPSolver>();

	ChainVRPSolver& chain_solver = am.createAlgorithm<ChainVRPSolver>();
	
	chain_solver.appendSolver(solver1);
	chain_solver.appendSolver(solver2);

	chain_solver.optimize(scene);

}