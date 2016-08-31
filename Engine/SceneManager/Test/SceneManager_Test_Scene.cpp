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
	
	
}

TEST_CASE("SceneManager - Scene - Stop")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	Scene* scene = sm->createScene(*scene_context);
}

