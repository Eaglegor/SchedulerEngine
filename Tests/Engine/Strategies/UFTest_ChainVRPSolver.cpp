#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include <Plugins/RoutingServices/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Schedule.h>
#include <Tests/Utils/Concepts/MakeLocation.h>

#include <Engine/SceneManager/Run.h>

#include <Engine/SceneManager/RunVehicleBinding/PerformerAssignedVehicleBinder.h>

#include <Engine/Strategies/StrategiesManager.h>
#include <Engine/Strategies/Strategy.h>
#include <Engine/Strategies/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>
#include <Engine/Strategies/VRPSolvers/Utilitary/Chain/ChainVRPSolver.h>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][vrp_solvers]")
{
    using namespace Scheduler;

    SceneManager sm(nullptr);

    Scene* scene = sm.createScene();

	StrategiesManager strategies_manager;

	Strategy* strategy = strategies_manager.createStrategy();

	TransparentVRPSolver* solver1 = strategy->createVRPSolver<TransparentVRPSolver>();
	TransparentVRPSolver* solver2 = strategy->createVRPSolver<TransparentVRPSolver>();

	ChainVRPSolver* chain_solver = strategy->createVRPSolver<ChainVRPSolver>();
	
	chain_solver->appendSolver(solver1);
	chain_solver->appendSolver(solver2);

	chain_solver->optimize(scene);

}