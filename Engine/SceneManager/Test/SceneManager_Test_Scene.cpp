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
#include <Engine/Utils/Collections/Algorithms.h>

using namespace Scheduler;

TEST_CASE("SceneManager - Scene - Construction")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	Scene* scene = sm->createScene(*scene_context);
	
	REQUIRE(scene != nullptr);
	REQUIRE(&scene->getContext() == scene_context);
	REQUIRE(scene->getSceneManager() == sm);
	REQUIRE(scene->getSchedules().empty());
}

TEST_CASE("SceneManager - Scene - Schedule")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();

	Performer* performer = scene_context->createPerformer();
	
	Scene* scene = sm->createScene(*scene_context);
	
	Schedule* schedule = scene->createSchedule(*performer);
	
	REQUIRE(schedule != nullptr);
	REQUIRE(std::string(schedule->getName()) == "");
	REQUIRE(schedule->getPerformer() == performer);
	REQUIRE(schedule->getRuns().empty());
	REQUIRE(schedule->getStops().empty());
	REQUIRE(schedule->isValid());
	REQUIRE(schedule->getActualizationModel() == nullptr);
	REQUIRE(schedule->getValidationModel() == nullptr);
	REQUIRE(schedule->getShift() == TimeWindow());
	REQUIRE(schedule->getScene() == scene);
	
	ScheduleActualizationModel* actualization_model = sm->createScheduleActualizationModel();
	schedule->setActualizationModel(actualization_model);
	
	ScheduleValidationModel* validation_model = sm->createScheduleValidationModel();
	schedule->setValidationModel(validation_model);
	
	schedule->setName("Schedule1");
	schedule->setShift(make_time_window(0, 4));
	
	REQUIRE(std::string(schedule->getName()) == "Schedule1");
	REQUIRE(schedule->getShift() == make_time_window(0, 4));
	REQUIRE(schedule->getActualizationModel() == actualization_model);
	REQUIRE(schedule->getValidationModel() == validation_model);
	
	const Location& start_location = *scene_context->createLocation(make_location(5,5));
	const Location& end_location = *scene_context->createLocation(make_location(5,6));
	
	Run* run1 = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
	Run* run0 = *schedule->createRun(schedule->getRuns().begin(), start_location, end_location);
	Run* run3 = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
	Run* run2 = *schedule->createRun(std::next(schedule->getRuns().begin(), 2), start_location, end_location);
	
	REQUIRE(run0 != nullptr);
	REQUIRE(run1 != nullptr);
	REQUIRE(run2 != nullptr);
	REQUIRE(run3 != nullptr);

	REQUIRE(schedule->getRuns().size() == 4);
	REQUIRE(schedule->getRuns()[0] == run0);
	REQUIRE(schedule->getRuns()[1] == run1);
	REQUIRE(schedule->getRuns()[2] == run2);
	REQUIRE(schedule->getRuns()[3] == run3);
	
	const Schedule* cschedule = schedule;
	REQUIRE(&schedule->constraints() == &cschedule->constraints());
}

TEST_CASE("SceneManager - Scene - Schedule - Constraints")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();

	Performer* performer = scene_context->createPerformer();
	
	const Location& start_location = *scene_context->createLocation(make_location(5,5));
	const Location& end_location = *scene_context->createLocation(make_location(5,6));
	
	Scene* scene = sm->createScene(*scene_context);
	
	Schedule* schedule = scene->createSchedule(*performer);
	
	REQUIRE_FALSE(schedule->constraints().scheduleDistanceLimit().isSet());
	REQUIRE_FALSE(schedule->constraints().runDistanceLimit().isSet());
	REQUIRE_FALSE(schedule->constraints().scheduleDrivingTimeLimit().isSet());
	REQUIRE_FALSE(schedule->constraints().runDrivingTimeLimit().isSet());
	REQUIRE_FALSE(schedule->constraints().scheduleWorkingTimeLimit().isSet());
	REQUIRE_FALSE(schedule->constraints().runWorkingTimeLimit().isSet());
	REQUIRE_FALSE(schedule->constraints().shiftStartLocation().isSet());
	REQUIRE_FALSE(schedule->constraints().shiftEndLocation().isSet());
	
	schedule->constraints().scheduleDistanceLimit().set(Distance(5));
	schedule->constraints().runDistanceLimit().set(Distance(6));
	schedule->constraints().scheduleDrivingTimeLimit().set(Duration(10));
	schedule->constraints().runDrivingTimeLimit().set(Duration(11));
	schedule->constraints().scheduleWorkingTimeLimit().set(Duration(12));
	schedule->constraints().runWorkingTimeLimit().set(Duration(13));
	schedule->constraints().shiftStartLocation().set(ScheduleConstraints::LocationConstraint(start_location));
	schedule->constraints().shiftEndLocation().set(ScheduleConstraints::LocationConstraint(end_location));
	
	REQUIRE(schedule->constraints().scheduleDistanceLimit().isSet());
	REQUIRE(schedule->constraints().scheduleDistanceLimit().get() == Distance(5));
	
	REQUIRE(schedule->constraints().runDistanceLimit().isSet());
	REQUIRE(schedule->constraints().runDistanceLimit().get() == Distance(6));

	REQUIRE(schedule->constraints().scheduleDrivingTimeLimit().isSet());
	REQUIRE(schedule->constraints().scheduleDrivingTimeLimit().get() == Duration(10));
	
	REQUIRE(schedule->constraints().runDrivingTimeLimit().isSet());
	REQUIRE(schedule->constraints().runDrivingTimeLimit().get() == Duration(11));
	
	REQUIRE(schedule->constraints().scheduleWorkingTimeLimit().isSet());
	REQUIRE(schedule->constraints().scheduleWorkingTimeLimit().get() == Duration(12));

	REQUIRE(schedule->constraints().runWorkingTimeLimit().isSet());
	REQUIRE(schedule->constraints().runWorkingTimeLimit().get() == Duration(13));
	
	REQUIRE(schedule->constraints().shiftStartLocation().isSet());
	REQUIRE(schedule->constraints().shiftStartLocation().get() == start_location);

	REQUIRE(schedule->constraints().shiftEndLocation().isSet());
	REQUIRE(schedule->constraints().shiftEndLocation().get() == end_location);
}

TEST_CASE("SceneManager - Scene - Run")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();

	Performer* performer = scene_context->createPerformer();
	
	Scene* scene = sm->createScene(*scene_context);
	
	Schedule* schedule = scene->createSchedule(*performer);
	
	const Location& start_location = *scene_context->createLocation(make_location(5,5));
	const Location& end_location = *scene_context->createLocation(make_location(5,6));
	
	Run* run = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
	
	REQUIRE(run != nullptr);
	REQUIRE(run->getSchedule() == schedule);
	REQUIRE(run->getStartStop() != nullptr);
	REQUIRE(run->getStartStop()->getLocation() == start_location);
	REQUIRE(run->getEndStop() != nullptr);
	REQUIRE(run->getEndStop()->getLocation() == end_location);
	REQUIRE(run->getWorkStops().empty());
	REQUIRE(run->isValid());
	REQUIRE(run->getVehicle() == nullptr);
	
	Vehicle* vehicle = scene_context->createVehicle();
	
	run->setVehicle(vehicle);
	
	REQUIRE(run->getVehicle() == vehicle);
}

TEST_CASE("SceneManager - Scene - Run - Boundary stops")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();

	Performer* performer = scene_context->createPerformer();
	
	Scene* scene = sm->createScene(*scene_context);
	
	Schedule* schedule = scene->createSchedule(*performer);
	
	const Location& start_location = *scene_context->createLocation(make_location(5,5));
	const Location& end_location = *scene_context->createLocation(make_location(5,6));
	
	Run* run = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
	
	REQUIRE(run->getStartStop()->getOperations().empty());
	REQUIRE(run->getEndStop()->getOperations().empty());
	REQUIRE(run->getWorkStops().empty());
	
	Operation* start_operation1 = scene_context->createFreeOperation(start_location);
	Operation* start_operation2 = scene_context->createFreeOperation(start_location);
	Operation* end_operation1 = scene_context->createFreeOperation(end_location);
	Operation* end_operation2 = scene_context->createFreeOperation(end_location);
	
	RunBoundaryStop* start_stop = run->allocateStartOperation(start_operation1);
	RunBoundaryStop* end_stop = run->allocateEndOperation(end_operation1);
	
	REQUIRE(run->getWorkStops().empty());
	
	REQUIRE(start_stop == run->getStartStop());
	REQUIRE(end_stop == run->getEndStop());
	
	start_stop = run->allocateStartOperation(start_operation2);
	end_stop = run->allocateEndOperation(end_operation2);
	
	REQUIRE(run->getWorkStops().empty());
	
	REQUIRE(start_stop == run->getStartStop());
	REQUIRE(end_stop == run->getEndStop());
	
	REQUIRE(start_stop->getOperations().size() == 2);
	REQUIRE(util::contains_key(start_stop->getOperations(), start_operation1));
	REQUIRE(util::contains_key(start_stop->getOperations(), start_operation2));
	
	REQUIRE(end_stop->getOperations().size() == 2);
	REQUIRE(util::contains_key(end_stop->getOperations(), end_operation1));
	REQUIRE(util::contains_key(end_stop->getOperations(), end_operation2));
	
	run->unallocateStartOperation(start_operation1);
	run->unallocateEndOperation(end_operation1);
	
	REQUIRE(start_stop->getOperations().size() == 1);
	REQUIRE(util::contains_key(start_stop->getOperations(), start_operation2));
	
	REQUIRE(end_stop->getOperations().size() == 1);
	REQUIRE(util::contains_key(end_stop->getOperations(), end_operation2));
	
	run->unallocateStartOperation(start_operation2);
	run->unallocateEndOperation(end_operation2);
	
	REQUIRE(start_stop->getOperations().empty());
	REQUIRE(start_stop->getOperations().empty());
}

TEST_CASE("SceneManager - Scene - Run - Work stops")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();

	Performer* performer = scene_context->createPerformer();
	
	Scene* scene = sm->createScene(*scene_context);
	
	Schedule* schedule = scene->createSchedule(*performer);
	
	const Location& start_location = *scene_context->createLocation(make_location(5,5));
	const Location& end_location = *scene_context->createLocation(make_location(5,6));
	
	const Location& work_location = *scene_context->createLocation(make_location(2,2));
	
	Run* run = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
	
	REQUIRE(run->getStartStop()->getOperations().empty());
	REQUIRE(run->getEndStop()->getOperations().empty());
	REQUIRE(run->getWorkStops().empty());
	
	Operation* work_operation1 = scene_context->createFreeOperation(work_location);
	Operation* work_operation2 = scene_context->createFreeOperation(work_location);
	Operation* work_operation3 = scene_context->createFreeOperation(work_location);
	Operation* work_operation4 = scene_context->createFreeOperation(work_location);
	Operation* work_operation5 = scene_context->createFreeOperation(work_location);

	WorkStop* work_stop3 = *run->createWorkStop(run->getWorkStops().end(), work_operation3);
	WorkStop* work_stop1 = *run->createWorkStop(run->getWorkStops().begin(), work_operation1);
	WorkStop* work_stop5 = *run->createWorkStop(run->getWorkStops().end(), work_operation5);
	WorkStop* work_stop2 = *run->createWorkStop(std::next(run->getWorkStops().begin()), work_operation2);
	WorkStop* work_stop4 = *run->createWorkStop(std::next(run->getWorkStops().begin(), 3), work_operation4);
	
	REQUIRE(work_stop1->getOperation() == work_operation1);
	REQUIRE(work_stop2->getOperation() == work_operation2);
	REQUIRE(work_stop3->getOperation() == work_operation3);
	REQUIRE(work_stop4->getOperation() == work_operation4);
	REQUIRE(work_stop5->getOperation() == work_operation5);
	
	REQUIRE(work_stop1->getLocation() == work_operation1->getLocation());
	REQUIRE(work_stop2->getLocation() == work_operation2->getLocation());
	REQUIRE(work_stop3->getLocation() == work_operation3->getLocation());
	REQUIRE(work_stop4->getLocation() == work_operation4->getLocation());
	REQUIRE(work_stop5->getLocation() == work_operation5->getLocation());
	
	REQUIRE(*std::next(run->getWorkStops().begin(), 0) == work_stop1);
	REQUIRE(*std::next(run->getWorkStops().begin(), 1) == work_stop2);
	REQUIRE(*std::next(run->getWorkStops().begin(), 2) == work_stop3);
	REQUIRE(*std::next(run->getWorkStops().begin(), 3) == work_stop4);
	REQUIRE(*std::next(run->getWorkStops().begin(), 4) == work_stop5);
	
	REQUIRE(run->getStartStop()->next() == work_stop1);
	REQUIRE(run->getStartStop() == work_stop1->prev());
	
	REQUIRE(work_stop1->next() == work_stop2);
	REQUIRE(work_stop1 == work_stop2->prev());
	
	REQUIRE(work_stop2->next() == work_stop3);
	REQUIRE(work_stop2 == work_stop3->prev());
	
	REQUIRE(work_stop3->next() == work_stop4);
	REQUIRE(work_stop3 == work_stop4->prev());
	
	REQUIRE(work_stop4->next() == work_stop5);
	REQUIRE(work_stop4 == work_stop5->prev());
	
	REQUIRE(work_stop5->next() == run->getEndStop());
	REQUIRE(work_stop5 == run->getEndStop()->prev());
	
	auto iter1 = std::next(run->getWorkStops().begin(), 0);
	auto iter2 = std::next(run->getWorkStops().begin(), 1);
	auto iter3 = std::next(run->getWorkStops().begin(), 2);
	auto iter4 = std::next(run->getWorkStops().begin(), 3);
	auto iter5 = std::next(run->getWorkStops().begin(), 4);
	
	run->destroyWorkStop(iter3);
	
	REQUIRE(*std::next(run->getWorkStops().begin(), 0) == work_stop1);
	REQUIRE(*std::next(run->getWorkStops().begin(), 1) == work_stop2);
	REQUIRE(*std::next(run->getWorkStops().begin(), 2) == work_stop4);
	REQUIRE(*std::next(run->getWorkStops().begin(), 3) == work_stop5);
	REQUIRE(run->getWorkStops().size() == 4);
	
	run->destroyWorkStop(iter1);
	
	REQUIRE(*std::next(run->getWorkStops().begin(), 0) == work_stop2);
	REQUIRE(*std::next(run->getWorkStops().begin(), 1) == work_stop4);
	REQUIRE(*std::next(run->getWorkStops().begin(), 2) == work_stop5);
	REQUIRE(run->getWorkStops().size() == 3);
	
	run->destroyWorkStop(iter5);
	
	REQUIRE(*std::next(run->getWorkStops().begin(), 0) == work_stop2);
	REQUIRE(*std::next(run->getWorkStops().begin(), 1) == work_stop4);
	REQUIRE(run->getWorkStops().size() == 2);
	
	run->destroyWorkStop(iter2);
	
	REQUIRE(*std::next(run->getWorkStops().begin(), 0) == work_stop4);
	REQUIRE(run->getWorkStops().size() == 1);
	
	run->destroyWorkStop(iter4);
	
	REQUIRE(run->getWorkStops().empty());
}


TEST_CASE("SceneManager - Scene - Run - Mutations")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();

	Performer* performer = scene_context->createPerformer();
	
	Scene* scene = sm->createScene(*scene_context);
	
	Schedule* schedule = scene->createSchedule(*performer);
	
	const Location& start_location = *scene_context->createLocation(make_location(5,5));
	const Location& end_location = *scene_context->createLocation(make_location(5,6));
	
	const Location& work_location = *scene_context->createLocation(make_location(2,2));
	
	Run* r = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
	
	REQUIRE(r->getStartStop()->getOperations().empty());
	REQUIRE(r->getEndStop()->getOperations().empty());
	REQUIRE(r->getWorkStops().empty());
	
	Operation* operation1 = scene_context->createFreeOperation(work_location);
	Operation* operation2 = scene_context->createFreeOperation(work_location);
	Operation* operation3 = scene_context->createFreeOperation(work_location);
	Operation* operation4 = scene_context->createFreeOperation(work_location);

	auto iter1 = r->createWorkStop(r->getWorkStops().end(), operation1);
	auto iter2 = r->createWorkStop(r->getWorkStops().end(), operation2);
	auto iter3 = r->createWorkStop(r->getWorkStops().end(), operation3);
	auto iter4 = r->createWorkStop(r->getWorkStops().end(), operation4);
	auto end = r->getWorkStops().end();
	
	SECTION("Swap")
	{
		SECTION("Direct order")
		{
			r->swapWorkStops(iter1, iter4);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
			
			r->swapWorkStops(iter2, iter3);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
			
			r->swapWorkStops(iter4, iter2);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
			
			r->swapWorkStops(iter3, iter1);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
			
			r->swapWorkStops(iter1, iter1);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		}
		SECTION("Reverse order")
		{
			r->swapWorkStops(iter4, iter1);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
			
			r->swapWorkStops(iter3, iter2);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
			
			r->swapWorkStops(iter2, iter4);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
			
			r->swapWorkStops(iter1, iter3);
			
			REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
			REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
			REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
			REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		}
	}
	
	SECTION("Reverse")
	{
		r->reverseWorkStops(iter1, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
	
		r->reverseWorkStops(iter4, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->reverseWorkStops(iter1, iter3);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->reverseWorkStops(iter3, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		
		r->reverseWorkStops(iter1, iter4);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		
		r->reverseWorkStops(iter1, iter1);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
	}
	
	SECTION("Self splice")
	{
		r->spliceOwnWorkStops(r->getWorkStops().begin(), iter3, iter4);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->spliceOwnWorkStops(r->getWorkStops().end(), iter1, iter2);

		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->spliceOwnWorkStops(r->getWorkStops().begin(), iter4, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation2);
		
		r->spliceOwnWorkStops(r->getWorkStops().end(), iter4, iter3);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->spliceOwnWorkStops(iter2, iter1, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->spliceOwnWorkStops(iter4, iter3, iter2);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
	}
}