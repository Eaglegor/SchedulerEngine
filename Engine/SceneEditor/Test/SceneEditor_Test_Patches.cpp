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
#include <Engine/SceneManager/WorkStop.h>


TEST_CASE("Scene editor patches", "[integration][functional][scene_editor]")
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
			REQUIRE(std::string("Operation") + std::to_string(expected_order[i] + 1) == (*std::next(run->getWorkStops().begin(), i))->getOperation()->getName());
		}
	};

	checkOrder(run, { 0, 1, 2, 3, 4 });

	SceneEditor editor;

	REQUIRE(editor.getCurrentVersion() == 0);

	auto run_iter = scene->getSchedules()[0]->getRuns().begin();

	auto perform = [&](size_t ia, size_t ib)
	{
		editor.performAction<SwapRunWorkStops>(run_iter, std::next(run->getWorkStops().begin(), ia), std::next(run->getWorkStops().begin(), ib));
	};
	
	auto patchPerform = [&](Patch &patch, size_t ia, size_t ib)
	{
		patch.performAction<SwapRunWorkStops>(run_iter, std::next(run->getWorkStops().begin(), ia), std::next(run->getWorkStops().begin(), ib));
	};
	
	MemoryManager m;
	
	SECTION("Default construction")
	{
		Patch p;
		REQUIRE(p.getCurrentVersion() == 0);
		REQUIRE(p.getBaseVersion() == 0);
		REQUIRE(p.getState() == Patch::State::NOT_INITIALIZED);
	}

	SECTION("Move construction")
	{
		Patch p2;
		
		p2.initialize(&m, 10);
		
		REQUIRE(p2.getCurrentVersion() == 10);
		REQUIRE(p2.getBaseVersion() == 10);
		REQUIRE(p2.getState() == Patch::State::OPEN);
		
		Patch p(std::move(p2));
		
		REQUIRE(p.getCurrentVersion() == 10);
		REQUIRE(p.getBaseVersion() == 10);
		REQUIRE(p.getState() == Patch::State::OPEN);
				
		REQUIRE(p2.getCurrentVersion() == 0);
		REQUIRE(p2.getBaseVersion() == 0);
		REQUIRE(p2.getState() == Patch::State::NOT_INITIALIZED);
	}
	
	SECTION("Move assignment")
	{
		Patch p2;
		
		p2.initialize(&m, 10);
		
		REQUIRE(p2.getCurrentVersion() == 10);
		REQUIRE(p2.getBaseVersion() == 10);
		REQUIRE(p2.getState() == Patch::State::OPEN);
		
		Patch p;
		
		REQUIRE(p.getCurrentVersion() == 0);
		REQUIRE(p.getBaseVersion() == 0);
		REQUIRE(p.getState() == Patch::State::NOT_INITIALIZED);
		
		p = std::move(p2);
		
		REQUIRE(p.getCurrentVersion() == 10);
		REQUIRE(p.getBaseVersion() == 10);
		REQUIRE(p.getState() == Patch::State::OPEN);
				
		REQUIRE(p2.getCurrentVersion() == 0);
		REQUIRE(p2.getBaseVersion() == 0);
		REQUIRE(p2.getState() == Patch::State::NOT_INITIALIZED);
	}
	
	SECTION("Hold & recall")
	{
		Patch p;
		
		p.initialize(&m, 0);
		
		patchPerform(p, 0, 1);
		
		checkOrder(run, {1, 0, 2, 3, 4});
		
		patchPerform(p, 1, 2);
		
		checkOrder(run, {1, 2, 0, 3, 4});
		
		p.hold();

		checkOrder(run, {0, 1, 2, 3, 4});
		REQUIRE(p.getState() == Patch::State::HELD);
		
		p.recall();
		
		checkOrder(run, {1, 2, 0, 3, 4});
		REQUIRE(p.getState() == Patch::State::OPEN);
	}
	
	SECTION("Versioning")
	{
		Patch p;
		
		p.initialize(&m, 0);
		
		REQUIRE(p.getCurrentVersion() == 0);
		REQUIRE(p.getBaseVersion() == 0);
		
		patchPerform(p, 0, 1);
		
		REQUIRE(p.getCurrentVersion() == 1);
		REQUIRE(p.getBaseVersion() == 0);
		
		patchPerform(p, 1, 2);
		
		REQUIRE(p.getCurrentVersion() == 2);
		REQUIRE(p.getBaseVersion() == 0);
		
		p.hold();
		
		REQUIRE(p.getCurrentVersion() == 2);
		REQUIRE(p.getBaseVersion() == 0);
		
		p.recall();
		
		REQUIRE(p.getCurrentVersion() == 2);
		REQUIRE(p.getBaseVersion() == 0);
	}
	
	SECTION("Construction by scene editor")
	{
		REQUIRE(editor.getCurrentVersion() == 0);
		
		perform(0,1);
		
		REQUIRE(editor.getCurrentVersion() == 1);
		
		Patch p = editor.createPatch();
		
		REQUIRE(p.getState() == Patch::State::OPEN);
		REQUIRE(p.getCurrentVersion() == 1);
		REQUIRE(p.getBaseVersion() == 1);
	}
	
	SECTION("Performing actions after scene editor")
	{
		REQUIRE(editor.getCurrentVersion() == 0);
		
		perform(0,1);
		
		checkOrder(run, {1, 0, 2, 3, 4});
		
		REQUIRE(editor.getCurrentVersion() == 1);
		
		Patch p = editor.createPatch();
		
		patchPerform(p, 1, 2);
		
		checkOrder(run, {1, 2, 0, 3, 4});
		
		REQUIRE(editor.getCurrentVersion() == 1);
		
		REQUIRE(p.getState() == Patch::State::OPEN);
		REQUIRE(p.getCurrentVersion() == 2);
		REQUIRE(p.getBaseVersion() == 1);
	}
	
	SECTION("Merging patch")
	{
		SECTION("Valid state")
		{
			REQUIRE(editor.getCurrentVersion() == 0);
		
			perform(0,1);
			
			checkOrder(run, {1, 0, 2, 3, 4});
			
			REQUIRE(editor.getCurrentVersion() == 1);
			
			Patch p = editor.createPatch();
			
			patchPerform(p, 1, 2);
			patchPerform(p, 2, 3);
			
			checkOrder(run, {1, 2, 3, 0, 4});
			
			REQUIRE(p.getState() == Patch::State::OPEN);
			REQUIRE(p.getCurrentVersion() == 3);
			REQUIRE(p.getBaseVersion() == 1);
			REQUIRE(editor.getCurrentVersion() == 1);
			
			std::size_t checkpoint_id;
			
			SECTION("Hold and apply")
			{
				p.hold();
				checkOrder(run, {1, 0, 2, 3, 4});
				checkpoint_id = editor.applyPatch(std::move(p));
			}
			SECTION("Direct apply")
			{
				checkpoint_id = editor.applyPatch(std::move(p));
			}
			
			checkOrder(run, {1, 2, 3, 0, 4});
			REQUIRE(editor.getCurrentVersion() == 3);
			
			REQUIRE(p.getState() == Patch::State::NOT_INITIALIZED);
			REQUIRE(p.getBaseVersion() == 0);
			REQUIRE(p.getCurrentVersion() == 0);
			
			editor.rollbackToCheckpoint(checkpoint_id);
			
			checkOrder(run, {1, 0, 2, 3, 4});
			REQUIRE(editor.getCurrentVersion() == 1);
		}
		SECTION("Not initialized patch")
		{
			REQUIRE(editor.getCurrentVersion() == 0);
		
			perform(0,1);
			
			checkOrder(run, {1, 0, 2, 3, 4});
			
			REQUIRE(editor.getCurrentVersion() == 1);
			
			Patch p = editor.createPatch();
			
			editor.applyPatch(std::move(p));
			
			REQUIRE(editor.getCurrentVersion() == 1);
			checkOrder(run, {1, 0, 2, 3, 4});
		}
		SECTION("Performing actions in patching state")
		{
			REQUIRE(editor.getCurrentVersion() == 0);

			perform(0,1);

			checkOrder(run, {1, 0, 2, 3, 4});
			
			REQUIRE(editor.getCurrentVersion() == 1);
			
			Patch p = editor.createPatch();
			
			patchPerform(p, 1, 2);
			
			checkOrder(run, {1, 2, 0, 3, 4});
			
			perform(2,3); // This action will have no effect because scene editor is in patching state
			
			checkOrder(run, {1, 2, 0, 3, 4});
			
			REQUIRE(editor.getCurrentVersion() == 1);
			
			editor.applyPatch(std::move(p));
			
			REQUIRE(editor.getCurrentVersion() == 2);
			checkOrder(run, {1, 2, 0, 3, 4});
			
			perform(2,3); // This action will have effect because scene editor is in open state
			
			checkOrder(run, {1, 2, 3, 0, 4});
			REQUIRE(editor.getCurrentVersion() == 3);
		}
		SECTION("Incorrect base version")
		{
			REQUIRE(editor.getCurrentVersion() == 0);

			perform(0,1);

			checkOrder(run, {1, 0, 2, 3, 4});
			
			REQUIRE(editor.getCurrentVersion() == 1);
			
			Patch p = editor.createPatch();
			
			patchPerform(p, 1, 2);
			
			checkOrder(run, {1, 2, 0, 3, 4});
			
			editor.abortPatching();
			
			perform(2,3);
			
			checkOrder(run, {1, 2, 3, 0, 4});
			
			REQUIRE(editor.getCurrentVersion() == 2);
			
			Patch p2 = editor.createPatch();
			editor.applyPatch(std::move(p)); // This action will have no effect because scene editor has different base version
			
			REQUIRE(editor.getCurrentVersion() == 2);
			checkOrder(run, {1, 2, 3, 0, 4});
		}
	}
	
}
	
