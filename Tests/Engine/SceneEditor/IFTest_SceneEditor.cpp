#include <sch_test_all>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>

#include <cstring>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/WorkStop.h>

void checkOrder(Scheduler::Run* run, std::vector<size_t> expected_order)
{
	for (int i = 0; i < run->getWorkStops().size(); ++i)
	{
		REQUIRE(std::string("Operation") + std::to_string(expected_order[i]) == run->getWorkStops()[i]->getOperation()->getName());
	}
}

TEST_CASE("Scene editor works", "[integration][functional][scene_editor]")
{
	using namespace Scheduler;

	CREATE_ENGINE(engine);

	JSONSceneLoader loader(engine.getSceneManager());

	Scene* scene = loader.loadScene("TestData/IFTest_SceneEditor/TestScene.json");

	Run* run = scene->getSchedules()[0]->getRuns()[0];

	const auto& work_stops = run->getWorkStops();

	checkOrder(run, { 1, 2, 3, 4, 5 });

	SceneEditor editor;

	SECTION("Direct action")
	{
		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[1], run->getWorkStops()[3]);

		checkOrder(run, {1, 4, 3, 2, 5});

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[4], run->getWorkStops()[0]);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[0], run->getWorkStops()[2]);

		checkOrder(run, { 3, 4, 5, 2, 1 });
	}

	SECTION("Single checkpoint rollback all")
	{
		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[1], run->getWorkStops()[3]);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[4], run->getWorkStops()[0]);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[0], run->getWorkStops()[2]);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackAll();

		checkOrder(run, { 1, 2, 3, 4, 5 });
	}

	SECTION("Rollback to last checkpoint")
	{
		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[1], run->getWorkStops()[3]);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint();

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[4], run->getWorkStops()[0]);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint();

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[0], run->getWorkStops()[2]);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 5, 4, 3, 2, 1 });
	}

	SECTION("Rollback to specific checkpoint")
	{
		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[1], run->getWorkStops()[3]);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint(); // 1

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[4], run->getWorkStops()[0]);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint(); // 2

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[0], run->getWorkStops()[2]);

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
		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[1], run->getWorkStops()[3]);

		checkOrder(run, { 1, 4, 3, 2, 5 });

		editor.checkpoint(); // 1

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[4], run->getWorkStops()[0]);

		checkOrder(run, { 5, 4, 3, 2, 1 });

		editor.checkpoint(); // 2

		editor.performAction<SwapRunWorkStops>(run, run->getWorkStops()[0], run->getWorkStops()[2]);

		checkOrder(run, { 3, 4, 5, 2, 1 });

		editor.clearHistory();

		editor.rollbackToLastCheckpoint();

		checkOrder(run, { 3, 4, 5, 2, 1 });
	}


}

TEST_CASE("Reverse action works", "[integration][functional][scene_editor]")
{
	using namespace Scheduler;

	CREATE_ENGINE(engine);

	JSONSceneLoader loader(engine.getSceneManager());

	Scene* scene = loader.loadScene("TestData/IFTest_SceneEditor/TestScene.json");

	Run* run = scene->getSchedules()[0]->getRuns()[0];

	const auto& work_stops = run->getWorkStops();

	checkOrder(run, { 1, 2, 3, 4, 5 });

	SceneEditor editor;

	editor.performAction<ReverseWorkStopsSubsequence>(run, 1, 4);

	checkOrder(run, { 1, 4, 3, 2, 5 });

	editor.performAction<ReverseWorkStopsSubsequence>(run, 0, 5);

	checkOrder(run, { 5, 2, 3, 4, 1 });

	editor.performAction<ReverseWorkStopsSubsequence>(run, 2, 5);

	checkOrder(run, { 5, 2, 1, 4, 3 });

	editor.performAction<ReverseWorkStopsSubsequence>(run, 0, 2);

	checkOrder(run, { 2, 5, 1, 4, 3 });

	editor.performAction<ReverseWorkStopsSubsequence>(run, 1, 2);

	checkOrder(run, { 2, 5, 1, 4, 3 });

	editor.rollbackToLastCheckpoint();

	checkOrder(run, { 1, 2, 3, 4, 5 });
}