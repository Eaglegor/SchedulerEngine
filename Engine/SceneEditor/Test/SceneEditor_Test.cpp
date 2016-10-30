#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>

#include <cstring>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneManager/WorkStop.h>


TEST_CASE("Scene editor works", "[integration][functional][scene_editor]")
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
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i]) == std::next(run.getWorkStops().begin(), i)->getOperation().getName());
		}
	};

	checkOrder(run, { 1, 2, 3, 4, 5 });

	SceneEditor editor;

	REQUIRE(editor.getCurrentVersion() == 0);

	SECTION("Direct action")
	{
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 1), std::next(run.getWorkStops().begin(), 3));

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, {1, 4, 3, 2, 5});

		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 4), std::next(run.getWorkStops().begin(), 0));

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 0), std::next(run.getWorkStops().begin(), 2));

		REQUIRE(editor.getCurrentVersion() == 3);
		
		checkOrder(run, { 3, 4, 5, 2, 1 });
	}

	SECTION("Single checkpoint rollback all")
	{
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 1), std::next(run.getWorkStops().begin(), 3));

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 4), std::next(run.getWorkStops().begin(), 0));

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 0), std::next(run.getWorkStops().begin(), 2));

		REQUIRE(editor.getCurrentVersion() == 3);
		
		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackAll();

		REQUIRE(editor.getCurrentVersion() == 0);
		
		checkOrder(run, { 1, 2, 3, 4, 5 });
	}

	SECTION("Rollback to last checkpoint")
	{
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 1), std::next(run.getWorkStops().begin(), 3));

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint();

		REQUIRE(editor.getCurrentVersion() == 1);
		
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 4), std::next(run.getWorkStops().begin(), 0));

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint();

		REQUIRE(editor.getCurrentVersion() == 2);
		
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 0), std::next(run.getWorkStops().begin(), 2));

		REQUIRE(editor.getCurrentVersion() == 3);
		
		checkOrder(run, { 3, 4, 5, 2, 1 });
		
		editor.rollbackToLastCheckpoint();

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.rollbackToLastCheckpoint();

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });
	}

	SECTION("Rollback to specific checkpoint")
	{
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 1), std::next(run.getWorkStops().begin(), 3));

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint(); // 1

		REQUIRE(editor.getCurrentVersion() == 1);
		
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 4), std::next(run.getWorkStops().begin(), 0));

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint(); // 2

		REQUIRE(editor.getCurrentVersion() == 2);
		
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 0), std::next(run.getWorkStops().begin(), 2));

		REQUIRE(editor.getCurrentVersion() == 3);
		
		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackToCheckpoint(1);

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, {1, 4, 3, 2, 5});

		editor.rollbackToLastCheckpoint();

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.rollbackToCheckpoint(0);

		REQUIRE(editor.getCurrentVersion() == 0);
		
		checkOrder(run, { 1, 2, 3, 4, 5 });

	}

	SECTION("Clear history")
	{
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 1), std::next(run.getWorkStops().begin(), 3));

		REQUIRE(editor.getCurrentVersion() == 1);
		
		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint(); // 1

		REQUIRE(editor.getCurrentVersion() == 1);
		
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 4), std::next(run.getWorkStops().begin(), 0));

		REQUIRE(editor.getCurrentVersion() == 2);
		
		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint(); // 2
		
		REQUIRE(editor.getCurrentVersion() == 2);
		
		editor.performAction<SwapWorkStops>(run, std::next(run.getWorkStops().begin(), 0), std::next(run.getWorkStops().begin(), 2));

		REQUIRE(editor.getCurrentVersion() == 3);
		
		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.commit();

		REQUIRE(editor.getCurrentVersion() == 3);
		
		editor.rollbackToLastCheckpoint();

		REQUIRE(editor.getCurrentVersion() == 3);
		
		checkOrder(run, { 3, 4, 5, 2, 1 });
	}


}
