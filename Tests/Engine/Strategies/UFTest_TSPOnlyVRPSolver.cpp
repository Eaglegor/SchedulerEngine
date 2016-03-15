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
#include <Engine/Strategies/VRPSolvers/Utilitary/TSPOnly/TSPOnlyVRPSolver.h>

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][vrp_solvers]")
{
    using namespace Scheduler;

    SceneManager sm(nullptr);

    Scene* scene = sm.createScene();

	StrategiesManager strategies_manager;

	Strategy* strategy = strategies_manager.createStrategy();

	class TestTSPSolver : public TSPSolver
	{
	public:
		virtual void optimize(Schedule* schedule){}
		virtual void optimize(Schedule* schedule, size_t index){}
	};

	TSPSolver* tsp_solver = strategy->createTSPSolver<TestTSPSolver>();

	TSPOnlyVRPSolver* solver = strategy->createVRPSolver<TSPOnlyVRPSolver>(tsp_solver);

	solver->optimize(scene);

}