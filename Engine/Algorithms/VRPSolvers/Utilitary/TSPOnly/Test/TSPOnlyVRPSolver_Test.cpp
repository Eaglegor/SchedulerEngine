#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/TSPOnly/TSPOnlyVRPSolver.h>

class MockTSPSolver : public Scheduler::TSPSolver
{
public:

	virtual void optimize(Scheduler::Schedule& schedule) const override
	{
		was_called = true;
	}

	virtual void optimize(Scheduler::Run& run) const override
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

    SceneManager& sm = engine.getSceneManager();

	SceneContext& scene_context = sm.createSceneContext();
	Performer& performer = scene_context.createPerformer();

	Scene& scene = sm.createScene(scene_context);
	Schedule& schedule = scene.createSchedule(performer);
	
	AlgorithmsManager& am = engine.getAlgorithmsManager();

	MockTSPSolver& tsp_solver = am.createAlgorithm<MockTSPSolver>();

	TSPOnlyVRPSolver& solver = am.createAlgorithm<TSPOnlyVRPSolver>(tsp_solver);

	solver.optimize(scene);

	REQUIRE(tsp_solver.wasCalled());
}