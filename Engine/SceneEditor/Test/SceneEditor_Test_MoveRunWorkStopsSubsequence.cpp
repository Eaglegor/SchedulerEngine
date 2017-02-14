#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>

#include <cstring>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneEditor/Actions/MoveWorkStops.h>
#include <Engine/SceneManager/WorkStop.h>

TEST_CASE("Engine/SceneEditor/MoveRunWorkStopsSubsequence", "[integration][functional][scene_editor]")
{
	using namespace Scheduler;

	Engine engine;
	CrowFlyRoutingService routing_service;

	JSONSceneLoader loader(engine, routing_service);

	Scene& scene = loader.loadScene("TestData/SceneEditor_Test/TestScene.json");

	Run& run = scene.getSchedules()[0].get().getRuns()[0];

	const auto& work_stops = run.getWorkStops();

	auto checkOrder = [&](Scheduler::Run& run, std::vector<std::size_t> expected_order)
	{
		for (int i = 0; i < run.getWorkStops().size(); ++i)
		{
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i] + 1) == std::next(run.getWorkStops().begin(), i)->getOperation().getName());
		}
	};

	checkOrder(run, { 0, 1, 2, 3, 4 });

	SceneEditor editor;

	auto perform = [&](std::size_t ia, std::size_t ib, std::size_t inew)
	{
		editor.performAction<MoveWorkStops>(run, std::next(run.getWorkStops().begin(), ia), std::next(run.getWorkStops().begin(), ib), std::next(run.getWorkStops().begin(), inew));
	};
	

	SECTION("Test1")
	{
		perform(1, 4, 0);

		checkOrder(run, {1, 2, 3, 0, 4 });
	}

	SECTION("Test2")
	{
		perform(1, 2, 0);

		checkOrder(run, { 1, 0, 2, 3, 4 });
	}

	SECTION("Test3")
	{
		perform(1, 2, 5);

		checkOrder(run, { 0, 2, 3, 4, 1 });
	}

	SECTION("Test4")
	{
		perform(1, 3, 5);

		checkOrder(run, { 0, 3, 4, 1, 2 });
	}

	/*
	SECTION("Test5") - Infeasible case
	{
		perform(1, 2, 1);

		checkOrder(run, { 0, 1, 2, 3, 4 });
	}
	*/

	/*
	SECTION("Test6") - Infeasible case
	{
		perform(1, 3, 1);

		checkOrder(run, { 0, 1, 2, 3, 4 });
	}
	*/

	SECTION("Test7")
	{
		perform(0, 1, 5);

		checkOrder(run, { 1, 2, 3, 4, 0 });
	}

	/*
	SECTION("Test8") - Infeasible case
	{
		perform(0, 5, 0);

		checkOrder(run, { 0, 1, 2, 3, 4 });
	}
	*/

	editor.rollbackAll();

	checkOrder(run, { 0, 1, 2, 3, 4 });
}
