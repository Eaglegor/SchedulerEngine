#include <catch.hpp>

#include <Engine/Engine/Engine.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/SceneManager.h>
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

    Location start_location = make_location(0, 0);
    Location end_location = make_location(0, 0.5);

    Location loc1 = make_location(0, 0.1);
    Location loc2 = make_location(0, 0.2);
    Location loc3 = make_location(0, 0.3);
    Location loc4 = make_location(0, 0.4);

    Scene* s = sm->createScene();


    Performer* performer = s->createPerformer();
    Vehicle* vehicle = s->createVehicle();

    Schedule* schedule = s->createSchedule(performer);
    schedule->setDepotLocation(start_location);
    schedule->constraints().shiftEndLocation().set(end_location);

	ScheduleActualizationModel* actualization_model = sm->createScheduleActualizationModel();
	DefaultRouteActualizationAlgorithm* route_actualization_algorithm = sm->createRouteActualizationAlgorithm<DefaultRouteActualizationAlgorithm>(&routing_service);
	actualization_model->setRouteActualizationAlgorithm(route_actualization_algorithm);
	DefaultDurationActualizationAlgorithm* duration_actualization_algorithm = sm->createDurationActualizationAlgorithm<DefaultDurationActualizationAlgorithm>();
	actualization_model->setDurationActualizationAlgorithm(duration_actualization_algorithm);
	
	schedule->setActualizationModel(actualization_model);

    Run* r = schedule->createRun(start_location, end_location);
    r->setVehicle(vehicle);

    SECTION("Simple duration check")
    {

        Duration dur = Units::minutes(10);

        Operation* sop = s->createFreeOperation();
        sop->setDuration(dur);
        sop->setLocation(start_location);

        Operation* sop2 = s->createFreeOperation();
        sop2->setDuration(dur);
        sop2->setLocation(start_location);

        Operation* op1 = s->createFreeOperation();
        op1->setDuration(dur);
        op1->setLocation(loc1);

        Operation* op2 = s->createFreeOperation();
        op2->setDuration(dur);
        op2->setLocation(loc2);

        Operation* op3 = s->createFreeOperation();
        op3->setDuration(dur);
        op3->setLocation(loc3);

        Operation* op4 = s->createFreeOperation();
        op4->setDuration(dur);
        op4->setLocation(loc4);

        Operation* eop = s->createFreeOperation();
        eop->setDuration(dur);
        eop->setLocation(end_location);

        r->allocateStartOperation(sop);
        r->allocateStartOperation(sop2);
        r->allocateEndOperation(eop);

        Stop *s1 = r->getStartStop();
        Stop *s2 = r->allocateWorkOperation(op1, 0);
        Stop *s3 = r->allocateWorkOperation(op2, 1);
        Stop *s4 = r->allocateWorkOperation(op3, 2);
        Stop *s5 = r->allocateWorkOperation(op4, 3);
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
