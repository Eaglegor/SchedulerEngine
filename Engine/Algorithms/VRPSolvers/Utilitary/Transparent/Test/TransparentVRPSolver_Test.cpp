#include <catch.hpp>

#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>

TEST_CASE("Strategies - VRPSolvers - Transparent", "[unit][functional][vrp_solvers]")
{
	using namespace Scheduler;

	CrowFlyRoutingService routing_service;

	Engine engine;

	SceneManager& sm = engine.getSceneManager( );

	Scene& scene = sm.createScene(sm.createSceneContext( ));

	AlgorithmsManager& am = engine.getAlgorithmsManager( );

	TransparentVRPSolver& solver = am.createAlgorithm<TransparentVRPSolver>( );

	solver.optimize(scene);
}