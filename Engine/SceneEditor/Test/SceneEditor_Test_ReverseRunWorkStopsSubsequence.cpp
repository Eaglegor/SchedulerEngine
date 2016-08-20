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
#include <Engine/SceneEditor/Actions/MoveRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/WorkStop.h>


TEST_CASE("Reverse action works", "[integration][functional][scene_editor]")
{
	auto checkOrder = [&](Scheduler::Run* run, std::vector<size_t> expected_order)
	{
		for (int i = 0; i < run->getWorkStops().size(); ++i)
		{
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i]) == (*std::next(run->getWorkStops().begin(), i))->getOperation()->getName());
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

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, std::next(run->getWorkStops().begin(), 1), std::next(run->getWorkStops().begin(), 4));

	checkOrder(run, { 1, 4, 3, 2, 5 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, std::next(run->getWorkStops().begin(), 0), std::next(run->getWorkStops().begin(), 5));

	checkOrder(run, { 5, 2, 3, 4, 1 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, std::next(run->getWorkStops().begin(), 2), std::next(run->getWorkStops().begin(), 5));

	checkOrder(run, { 5, 2, 1, 4, 3 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, std::next(run->getWorkStops().begin(), 0), std::next(run->getWorkStops().begin(), 2));

	checkOrder(run, { 2, 5, 1, 4, 3 });

	editor.performAction<ReverseWorkStopsSubsequence>(run_iter, std::next(run->getWorkStops().begin(), 1), std::next(run->getWorkStops().begin(), 2));

	checkOrder(run, { 2, 5, 1, 4, 3 });

	editor.rollbackToLastCheckpoint();

	checkOrder(run, { 1, 2, 3, 4, 5 });
}