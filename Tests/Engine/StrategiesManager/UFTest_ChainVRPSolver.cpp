#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Schedule.h>
#include <Tests/Utils/MakeLocation.h>

#include <Engine/SceneManager/Run.h>

#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>

#include <Engine/StrategiesManager/StrategiesManager.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Chain/ChainVRPSolver.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/EngineContext.h>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][vrp_solvers]")
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