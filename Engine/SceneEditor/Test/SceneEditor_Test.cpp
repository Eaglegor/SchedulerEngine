#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>

#include <cstring>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/WorkStop.h>


TEST_CASE("Scene editor works", "[integration][functional][scene_editor]")
{
	using namespace Scheduler;

	Engine engine;
	CrowFlyRoutingService routing_service;

	JSONSceneLoader loader(engine.getSceneManager(), &routing_service);

	Scene* scene = loader.loadScene("TestData/SceneEditor_Test/TestScene.json");

	Run* run = scene->getSchedules()[0]->getRuns()[0];

	const auto& work_stops = run->getWorkStops();

	auto checkOrder = [&](Scheduler::Run* run, std::vector<size_t> expected_order)
	{
		for (int i = 0; i < run->getWorkStops().size(); ++i)
		{
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i]) == run->getWorkStops()[i]->getOperation()->getName());
		}
	};

	checkOrder(run, { 1, 2, 3, 4, 5 });

	SceneEditor editor;

	auto run_iter = scene->getSchedules()[0]->getRuns().begin();

	SECTION("Direct action")
	{
		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 3);

		checkOrder(run, {1, 4, 3, 2, 5});

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 4, run->getWorkStops().begin() + 0);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 2);

		checkOrder(run, { 3, 4, 5, 2, 1 });
	}

	SECTION("Single checkpoint rollback all")
	{
		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 3);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 4, run->getWorkStops().begin() + 0);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 2);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackAll();

		checkOrder(run, { 1, 2, 3, 4, 5 });
	}

	SECTION("Rollback to last checkpoint")
	{
		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 3);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint();

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 4, run->getWorkStops().begin() + 0);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint();

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 2);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 5, 4, 3, 2, 1 });
	}

	SECTION("Rollback to specific checkpoint")
	{
		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 3);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint(); // 1

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 4, run->getWorkStops().begin() + 0);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint(); // 2

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 2);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackToCheckpoint(1);

		checkOrder(run, {1, 4, 3, 2, 5});

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.rollbackToCheckpoint(0);

		checkOrder(run, { 1, 2, 3, 4, 5 });

	}

	SECTION("Clear history")
	{
		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 3);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint(); // 1

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 4, run->getWorkStops().begin() + 0);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint(); // 2

		editor.performAction<SwapRunWorkStops>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 2);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.commit();

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 3, 4, 5, 2, 1 });
	}


}

TEST_CASE("Reverse action works", "[integration][functional][scene_editor]")
{
	auto checkOrder = [&](Scheduler::Run* run, std::vector<size_t> expected_order)
	{
		for (int i = 0; i < run->getWorkStops().size(); ++i)
		{
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i]) == run->getWorkStops()[i]->getOperation()->getName());
		}
	};

	using namespace Scheduler;

	Engine engine;
	CrowFlyRoutingService routing_service;

	JSONSceneLoader loader(engine.getSceneManager(), &routing_service);

	Scene* scene = loader.loadScene("TestData/SceneEditor_Test/TestScene.json");

	Run* run = scene->getSchedules()[0]->getRuns()[0];

	const auto& work_stops = run->getWorkStops();

	checkOrder(run, { 1, 2, 3, 4, 5 });

	SceneEditor editor;

	auto run_iter = scene->getSchedules()[0]->getRuns().begin();

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 4);

	checkOrder(run, { 1, 4, 3, 2, 5 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 5);

	checkOrder(run, { 5, 2, 3, 4, 1 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, run->getWorkStops().begin() + 2, run->getWorkStops().begin() + 5);

	checkOrder(run, { 5, 2, 1, 4, 3 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, run->getWorkStops().begin() + 0, run->getWorkStops().begin() + 2);

	checkOrder(run, { 2, 5, 1, 4, 3 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, run->getWorkStops().begin() + 1, run->getWorkStops().begin() + 2);

	checkOrder(run, { 2, 5, 1, 4, 3 });

	editor.rollbackToLastCheckpoint();

	checkOrder(run, { 1, 2, 3, 4, 5 });
}