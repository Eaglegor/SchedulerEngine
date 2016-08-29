#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/StrategiesManager/StrategiesManager.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][functional][vrp_solvers]")
{
    using namespace Scheduler;

	CrowFlyRoutingService routing_service;

	Engine engine;

    SceneManager *sm = engine.getSceneManager();

    Scene* scene = sm->createScene(*sm->createSceneContext());

	StrategiesManager *strategies_manager = engine.getStrategiesManager();

	Strategy* strategy = strategies_manager->createStrategy();

	TransparentVRPSolver* solver = strategy->createVRPSolver<TransparentVRPSolver>();

	solver->optimize(scene);

}