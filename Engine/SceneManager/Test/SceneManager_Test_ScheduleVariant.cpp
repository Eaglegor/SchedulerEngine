#include <catch.hpp>

#include <Engine/Concepts/Test/MakeTimeWindow.h>
#include <Engine/Concepts/Test/MakeLocation.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/ScheduleVariant.h>
#include <Engine/Utils/Collections/Algorithms.h>

using namespace Scheduler;

TEST_CASE("SceneManager - ScheduleVariant")
{
	Engine engine;
	
	SceneManager& sm = engine.getSceneManager();
	
	SceneContext& scene_context = sm.createSceneContext();
	
	Performer& performer = scene_context.createPerformer();
	Vehicle& vehicle = scene_context.createVehicle();
	
	const Location& loc = scene_context.createLocation(make_location(5, 6));
	
	Operation& operation1 = scene_context.createFreeOperation(loc);
	Operation& operation2 = scene_context.createFreeOperation(loc);
	Operation& operation3 = scene_context.createFreeOperation(loc);
	Operation& operation4 = scene_context.createFreeOperation(loc);
	Operation& operationS1 = scene_context.createFreeOperation(loc);
	Operation& operationE1 = scene_context.createFreeOperation(loc);
	Operation& operationS2 = scene_context.createFreeOperation(loc);
	Operation& operationE2 = scene_context.createFreeOperation(loc);
	
	Scene& scene = sm.createScene(scene_context);
	
	Schedule& schedule = scene.createSchedule(performer);
	Run& run1 = *schedule.createRun(schedule.getRuns().end(), loc, loc);
	Run& run2 = *schedule.createRun(schedule.getRuns().end(), loc, loc);
	
	run1.allocateStartOperation(operationS1);
	run1.allocateEndOperation(operationE1);
	run2.allocateStartOperation(operationS2);
	run2.allocateEndOperation(operationE2);
	
	run1.createWorkStop(run1.getWorkStops().end(), operation1);
	run1.createWorkStop(run1.getWorkStops().end(), operation2);
	
	run2.createWorkStop(run2.getWorkStops().end(), operation3);
	run2.createWorkStop(run2.getWorkStops().end(), operation4);
	
	REQUIRE(schedule.getRuns().size() == 2);
	
	REQUIRE(schedule.getRuns()[0].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[0].get().getStartStop().getOperations(), operationS1));

	REQUIRE(schedule.getRuns()[0].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[0].get().getEndStop().getOperations(), operationE1));

	REQUIRE(schedule.getRuns()[0].get().getWorkStops().size() == 2);
	REQUIRE(std::next(schedule.getRuns()[0].get().getWorkStops().begin(), 0)->getOperation() == operation1);
	REQUIRE(std::next(schedule.getRuns()[0].get().getWorkStops().begin(), 1)->getOperation() == operation2);
	
	
	REQUIRE(schedule.getRuns()[1].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[1].get().getStartStop().getOperations(), operationS2));

	REQUIRE(schedule.getRuns()[1].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[1].get().getEndStop().getOperations(), operationE2));

	REQUIRE(schedule.getRuns()[1].get().getWorkStops().size() == 2);
	REQUIRE(std::next(schedule.getRuns()[1].get().getWorkStops().begin(), 0)->getOperation() == operation3);
	REQUIRE(std::next(schedule.getRuns()[1].get().getWorkStops().begin(), 1)->getOperation() == operation4);
	
	ScheduleVariant variant = schedule;
	
	Schedule& cloned_schedule = variant.getSchedule().value();
	
	REQUIRE(cloned_schedule.getScene() != schedule.getScene());
	REQUIRE(cloned_schedule.getScene().getContext() == schedule.getScene().getContext());
	REQUIRE(cloned_schedule.getPerformer() == schedule.getPerformer());
	
	REQUIRE(cloned_schedule.getRuns().size() == 2);
	
	REQUIRE(cloned_schedule.getRuns()[0].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[0].get().getStartStop().getOperations(), operationS1));

	REQUIRE(cloned_schedule.getRuns()[0].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[0].get().getEndStop().getOperations(), operationE1));

	REQUIRE(cloned_schedule.getRuns()[0].get().getWorkStops().size() == 2);
	REQUIRE(std::next(cloned_schedule.getRuns()[0].get().getWorkStops().begin(), 0)->getOperation() == operation1);
	REQUIRE(std::next(cloned_schedule.getRuns()[0].get().getWorkStops().begin(), 1)->getOperation() == operation2);
	
	
	REQUIRE(cloned_schedule.getRuns()[1].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[1].get().getStartStop().getOperations(), operationS2));

	REQUIRE(cloned_schedule.getRuns()[1].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[1].get().getEndStop().getOperations(), operationE2));

	REQUIRE(cloned_schedule.getRuns()[1].get().getWorkStops().size() == 2);
	REQUIRE(std::next(cloned_schedule.getRuns()[1].get().getWorkStops().begin(), 0)->getOperation() == operation3);
	REQUIRE(std::next(cloned_schedule.getRuns()[1].get().getWorkStops().begin(), 1)->getOperation() == operation4);
	
	
	cloned_schedule.getRuns()[0].get().destroyWorkStop(cloned_schedule.getRuns()[0].get().getWorkStops().begin());
	cloned_schedule.getRuns()[0].get().destroyWorkStop(cloned_schedule.getRuns()[0].get().getWorkStops().begin());
	cloned_schedule.getRuns()[1].get().destroyWorkStop(cloned_schedule.getRuns()[1].get().getWorkStops().begin());
	cloned_schedule.getRuns()[1].get().destroyWorkStop(cloned_schedule.getRuns()[1].get().getWorkStops().begin());
	
	cloned_schedule.getRuns()[0].get().createWorkStop(cloned_schedule.getRuns()[0].get().getWorkStops().end(), operation3);
	cloned_schedule.getRuns()[0].get().createWorkStop(cloned_schedule.getRuns()[0].get().getWorkStops().end(), operation4);
	
	cloned_schedule.getRuns()[1].get().createWorkStop(cloned_schedule.getRuns()[1].get().getWorkStops().end(), operation1);
	cloned_schedule.getRuns()[1].get().createWorkStop(cloned_schedule.getRuns()[1].get().getWorkStops().end(), operation2);
	
	
	REQUIRE(cloned_schedule.getRuns().size() == 2);
	
	REQUIRE(cloned_schedule.getRuns()[1].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[1].get().getStartStop().getOperations(), operationS2));

	REQUIRE(cloned_schedule.getRuns()[1].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[1].get().getEndStop().getOperations(), operationE2));

	REQUIRE(cloned_schedule.getRuns()[1].get().getWorkStops().size() == 2);
	REQUIRE(std::next(cloned_schedule.getRuns()[1].get().getWorkStops().begin(), 0)->getOperation() == operation1);
	REQUIRE(std::next(cloned_schedule.getRuns()[1].get().getWorkStops().begin(), 1)->getOperation() == operation2);
	
	
	REQUIRE(cloned_schedule.getRuns()[0].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[0].get().getStartStop().getOperations(), operationS1));

	REQUIRE(cloned_schedule.getRuns()[0].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(cloned_schedule.getRuns()[0].get().getEndStop().getOperations(), operationE1));

	REQUIRE(cloned_schedule.getRuns()[0].get().getWorkStops().size() == 2);
	REQUIRE(std::next(cloned_schedule.getRuns()[0].get().getWorkStops().begin(), 0)->getOperation() == operation3);
	REQUIRE(std::next(cloned_schedule.getRuns()[0].get().getWorkStops().begin(), 1)->getOperation() == operation4);
	
	REQUIRE(schedule.getRuns().size() == 2);
	
	REQUIRE(schedule.getRuns()[0].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[0].get().getStartStop().getOperations(), operationS1));

	REQUIRE(schedule.getRuns()[0].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[0].get().getEndStop().getOperations(), operationE1));

	REQUIRE(schedule.getRuns()[0].get().getWorkStops().size() == 2);
	REQUIRE(std::next(schedule.getRuns()[0].get().getWorkStops().begin(), 0)->getOperation() == operation1);
	REQUIRE(std::next(schedule.getRuns()[0].get().getWorkStops().begin(), 1)->getOperation() == operation2);
	
	
	REQUIRE(schedule.getRuns()[1].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[1].get().getStartStop().getOperations(), operationS2));

	REQUIRE(schedule.getRuns()[1].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[1].get().getEndStop().getOperations(), operationE2));

	REQUIRE(schedule.getRuns()[1].get().getWorkStops().size() == 2);
	REQUIRE(std::next(schedule.getRuns()[1].get().getWorkStops().begin(), 0)->getOperation() == operation3);
	REQUIRE(std::next(schedule.getRuns()[1].get().getWorkStops().begin(), 1)->getOperation() == operation4);
	
	variant.apply();
	
	REQUIRE(schedule.getRuns().size() == 2);
	
	REQUIRE(schedule.getRuns()[1].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[1].get().getStartStop().getOperations(), operationS2));

	REQUIRE(schedule.getRuns()[1].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[1].get().getEndStop().getOperations(), operationE2));

	REQUIRE(schedule.getRuns()[1].get().getWorkStops().size() == 2);
	REQUIRE(std::next(schedule.getRuns()[1].get().getWorkStops().begin(), 0)->getOperation() == operation1);
	REQUIRE(std::next(schedule.getRuns()[1].get().getWorkStops().begin(), 1)->getOperation() == operation2);
	
	
	REQUIRE(schedule.getRuns()[0].get().getStartStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[0].get().getStartStop().getOperations(), operationS1));

	REQUIRE(schedule.getRuns()[0].get().getEndStop().getOperations().size() == 1);
	REQUIRE(util::contains_key(schedule.getRuns()[0].get().getEndStop().getOperations(), operationE1));

	REQUIRE(schedule.getRuns()[0].get().getWorkStops().size() == 2);
	REQUIRE(std::next(schedule.getRuns()[0].get().getWorkStops().begin(), 0)->getOperation() == operation3);
	REQUIRE(std::next(schedule.getRuns()[0].get().getWorkStops().begin(), 1)->getOperation() == operation4);
	
}