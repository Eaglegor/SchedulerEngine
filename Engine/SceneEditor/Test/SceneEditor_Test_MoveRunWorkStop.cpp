#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>

#include <cstring>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/SceneManager/WorkStop.h>

TEST_CASE("Engine/SceneEditor/MoveRunWorkStop", "[integration][functional][scene_editor]")
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
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i] + 1) == run->getWorkStops()[i]->getOperation()->getName());
		}
	};

	checkOrder(run, { 0, 1, 2, 3, 4 });

	SceneEditor editor;

	auto run_iter = scene->getSchedules()[0]->getRuns().begin();

	auto perform = [&](size_t ia, size_t ib)
	{
		editor.performAction<MoveRunWorkStop>(run_iter, run->getWorkStops().begin() + ia, run->getWorkStops().begin() + ib);
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

	SECTION("Test3")
	{
		perform(1, 1);

		checkOrder(run, { 0, 1, 2, 3, 4 });
	}

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
