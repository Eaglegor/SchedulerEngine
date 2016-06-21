#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/StrategiesManager/StrategiesManager.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/TSPOnly/TSPOnlyVRPSolver.h>

class MockTSPSolver : public Scheduler::TSPSolver
{
public:

	virtual void optimize(Scheduler::Schedule* schedule) const override
	{
		was_called = true;
	}

	virtual void optimize(Scheduler::Run* run) const override
	{
		was_called = true;
	}

	virtual const char* getName() const override
	{
		return "Mock";
	}

	bool wasCalled()
	{
		return was_called;
	}

private:
	mutable bool was_called = false;
};

TEST_CASE("Strategies - VRPSolvers - DummyVRPSolver", "[unit][functional][vrp_solvers]")
{
    using namespace Scheduler;

	CrowFlyRoutingService routing_service;

	Engine engine;

    SceneManager* sm = engine.getSceneManager();

    Scene* scene = sm->createScene();
	Performer* performer = scene->createPerformer();
	Schedule* schedule = scene->createSchedule(performer);

	StrategiesManager* strategies_manager = engine.getStrategiesManager();

	Strategy* strategy = strategies_manager->createStrategy();

	MockTSPSolver* tsp_solver = strategy->createTSPSolver<MockTSPSolver>();

	TSPOnlyVRPSolver* solver = strategy->createVRPSolver<TSPOnlyVRPSolver>(tsp_solver);

	solver->optimize(scene);

	REQUIRE(tsp_solver->wasCalled());
}