#include <catch.hpp>

#include <Engine/Engine/Engine.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>
#include <Engine/Concepts/Test/MakeLocation.h>
#include <Engine/Algorithms/ScheduleActualization/Route/Default/DefaultRouteActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/Duration/Default/DefaultDurationActualizationAlgorithm.h>
#include <Engine/Utils/Units/DurationUnits.h>

TEST_CASE("ScheduleActualizers - StopDurationActualizationAlgorithm", "[integration][functional][schedule_actualizers]")
{
     using namespace Scheduler;

    CrowFlyRoutingService routing_service;

	Engine engine;
    SceneManager* sm = engine.getSceneManager();

	SceneContext* scene_context = sm->createSceneContext();
	
    const Location& start_location = *scene_context->createLocation(make_location(0, 0));
    const Location& end_location = *scene_context->createLocation(make_location(0, 0.5));

    const Location& loc1 = *scene_context->createLocation(make_location(0, 0.1));
    const Location& loc2 = *scene_context->createLocation(make_location(0, 0.2));
    const Location& loc3 = *scene_context->createLocation(make_location(0, 0.3));
    const Location& loc4 = *scene_context->createLocation(make_location(0, 0.4));
	
    Performer* performer = scene_context->createPerformer();
	performer->setDepot(scene_context->createDepot(start_location));
    Vehicle* vehicle = scene_context->createVehicle();

	Scene* s = sm->createScene(*scene_context);
	
    Schedule* schedule = s->createSchedule(*performer);

	ScheduleActualizationModel* actualization_model = sm->createScheduleActualizationModel();
	DefaultRouteActualizationAlgorithm* route_actualization_algorithm = sm->createRouteActualizationAlgorithm<DefaultRouteActualizationAlgorithm>(&routing_service);
	actualization_model->setRouteActualizationAlgorithm(route_actualization_algorithm);
	DefaultDurationActualizationAlgorithm* duration_actualization_algorithm = sm->createDurationActualizationAlgorithm<DefaultDurationActualizationAlgorithm>();
	actualization_model->setDurationActualizationAlgorithm(duration_actualization_algorithm);
	
	schedule->setActualizationModel(actualization_model);

    Run* r = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
    r->setVehicle(vehicle);

    SECTION("Simple duration check")
    {

        Duration dur = Units::minutes(10);

        Operation* sop = scene_context->createFreeOperation(start_location);
        sop->setDuration(dur);

        Operation* sop2 = scene_context->createFreeOperation(start_location);
        sop2->setDuration(dur);

        Operation* op1 = scene_context->createFreeOperation(loc1);
        op1->setDuration(dur);

        Operation* op2 = scene_context->createFreeOperation(loc2);
        op2->setDuration(dur);

        Operation* op3 = scene_context->createFreeOperation(loc3);
        op3->setDuration(dur);

        Operation* op4 = scene_context->createFreeOperation(loc4);
        op4->setDuration(dur);

        Operation* eop = scene_context->createFreeOperation(end_location);
        eop->setDuration(dur);

        r->allocateStartOperation(sop);
        r->allocateStartOperation(sop2);
        r->allocateEndOperation(eop);

        Stop *s1 = r->getStartStop();
        Stop *s2 = *r->createWorkStop(r->getWorkStops().end(), op1);
        Stop *s3 = *r->createWorkStop(r->getWorkStops().end(), op2);
        Stop *s4 = *r->createWorkStop(r->getWorkStops().end(), op3);
        Stop *s5 = *r->createWorkStop(r->getWorkStops().end(), op4);
        Stop *s6 = r->getEndStop();

        REQUIRE(s1->getDuration() == dur*2);
        REQUIRE(s2->getDuration() == dur);
        REQUIRE(s3->getDuration() == dur);
        REQUIRE(s4->getDuration() == dur);
        REQUIRE(s5->getDuration() == dur);
        REQUIRE(s6->getDuration() == dur);
    }

    sm->destroyScene(s);
}
