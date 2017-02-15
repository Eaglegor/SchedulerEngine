#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>

#include <cstring>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/MoveWorkStop.h>
#include <Engine/SceneManager/WorkStop.h>

TEST_CASE("Engine/SceneEditor/MoveRunWorkStop", "[integration][functional][scene_editor]")
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

	auto perform = [&](std::size_t ia, std::size_t ib)
	{
		editor.performAction<MoveWorkStop>(run, std::next(run.getWorkStops().begin(), ia), std::next(run.getWorkStops().begin(), ib));
	};

	SECTION("Test1")
	{
		perform(1, 0);

		checkOrder(run, {1, 0, 2, 3, 4 });
	}

	SECTION("Test2")
	{
		perform(1, 5);

		checkOrder(run, { 0, 2, 3, 4, 1 });
	}

	/*
	 * SECTION("Test3") - Infeasible case
	{
		perform(1, 1);

		checkOrder(run, { 0, 1, 2, 3, 4 });
	}
	*/

	SECTION("Test4")
	{
		perform(0, 5);

		checkOrder(run, { 1, 2, 3, 4, 0 });
	}

	SECTION("Test5")
	{
		perform(0, 2);

		checkOrder(run, { 1, 0, 2, 3, 4 });
	}

	editor.rollbackAll();

	checkOrder(run, { 0, 1, 2, 3, 4 });
}
