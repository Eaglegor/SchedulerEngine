#include <catch.hpp>

#include <Engine/Engine/Engine.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>
#include <Engine/Concepts/Test/MakeLocation.h>

TEST_CASE("ScheduleActualizers - StopDurationActualizationAlgorithm", "[integration][functional][schedule_actualizers]")
{
    using namespace Scheduler;

    CrowFlyRoutingService routing_service;

	EngineContext context;
	context.routing_service = &routing_service;
	Engine engine(context);
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
    schedule->setShiftEndLocation(end_location);

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

TEST_CASE("ScheduleActualizers - StopArrivalTimeActualizationAlgorithm", "[integration][schedule_actualizers][!mayfail]") // Temporary may fail (different crowfly routing service results on different platforms)
{
    using namespace Scheduler;

    CrowFlyRoutingService routing_service;

	EngineContext context;
	context.routing_service = &routing_service;
	Engine engine(context);
	SceneManager* sm = engine.getSceneManager();

    Location start_location = make_location(0, 0);
    Location end_location = make_location(0, 0.5);

    Location loc1 = make_location(0, 0.1);
    Location loc2 = make_location(0, 0.2);
    Location loc3 = make_location(0, 0.3);
    Location loc4 = make_location(0, 0.4);

    Scene* s = sm->createScene();

    Duration dur = Units::minutes(10);

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

    Performer* performer = s->createPerformer();
    Vehicle* vehicle = s->createVehicle();

    Schedule* schedule = s->createSchedule(performer);
    schedule->setDepotLocation(start_location);
    schedule->setShiftEndLocation(end_location);

    Run* r = schedule->createRun(start_location, end_location);
    r->setVehicle(vehicle);

    Route r1 = routing_service.calculateRoute(start_location, loc1, vehicle->getRoutingProfile());
    Route r2 = routing_service.calculateRoute(loc1, loc2, vehicle->getRoutingProfile());
    Route r3 = routing_service.calculateRoute(loc2, loc3, vehicle->getRoutingProfile());
    Route r4 = routing_service.calculateRoute(loc3, loc4, vehicle->getRoutingProfile());
    Route r5 = routing_service.calculateRoute(loc4, end_location, vehicle->getRoutingProfile());

	ScheduleActualizationModel* actualization_model = sm->createScheduleActualizationModel();
	DefaultRouteActualizationAlgorithm* route_actualization_algorithm = sm->createRouteActualizationAlgorithm<DefaultRouteActualizationAlgorithm>(&routing_service);
	actualization_model->setRouteActualizationAlgorithm(route_actualization_algorithm);
	DefaultDurationActualizationAlgorithm* duration_actualization_algorithm = sm->createDurationActualizationAlgorithm<DefaultDurationActualizationAlgorithm>();
	actualization_model->setDurationActualizationAlgorithm(duration_actualization_algorithm);

    schedule->setActualizationModel(actualization_model);

    TimeWindow estimated_allocation_1;
    TimeWindow estimated_allocation_2;
    TimeWindow estimated_allocation_3;
    TimeWindow estimated_allocation_4;
    TimeWindow estimated_allocation_5;
    TimeWindow estimated_allocation_6;

    SECTION("Broad time windows") {
        estimated_allocation_1.setStartTime(TimePoint(0));
        estimated_allocation_1.setEndTime(TimePoint(0));

        estimated_allocation_2.setStartTime(estimated_allocation_1.getEndTime() + r1.getDuration());
        estimated_allocation_2.setEndTime(estimated_allocation_2.getStartTime() + dur);

        estimated_allocation_3.setStartTime(estimated_allocation_2.getEndTime() + r2.getDuration());
        estimated_allocation_3.setEndTime(estimated_allocation_3.getStartTime() + dur);

        estimated_allocation_4.setStartTime(estimated_allocation_3.getEndTime() + r3.getDuration());
        estimated_allocation_4.setEndTime(estimated_allocation_4.getStartTime() + dur);

        estimated_allocation_5.setStartTime(estimated_allocation_4.getEndTime() + r4.getDuration());
        estimated_allocation_5.setEndTime(estimated_allocation_5.getStartTime() + dur);

        estimated_allocation_6.setStartTime(estimated_allocation_5.getEndTime() + r5.getDuration());
        estimated_allocation_6.setEndTime(estimated_allocation_6.getStartTime());
    }

    SECTION("Narrow time windows")
    {
        op1->setTimeWindows({make_time_window(770, 1370)});
        op2->setTimeWindows({make_time_window(2140, 2740)});
        op3->setTimeWindows({make_time_window(3510, 4110)});
        op4->setTimeWindows({make_time_window(4880, 5480)});

        estimated_allocation_1.setStartTime(TimePoint(100));
        estimated_allocation_1.setEndTime(TimePoint(100));

        estimated_allocation_2.setStartTime(TimePoint(770));
        estimated_allocation_2.setEndTime(estimated_allocation_2.getStartTime() + dur);

        estimated_allocation_3.setStartTime(TimePoint(2140));
        estimated_allocation_3.setEndTime(estimated_allocation_3.getStartTime() + dur);

        estimated_allocation_4.setStartTime(TimePoint(3510));
        estimated_allocation_4.setEndTime(estimated_allocation_4.getStartTime() + dur);

        estimated_allocation_5.setStartTime(TimePoint(4880));
        estimated_allocation_5.setEndTime(estimated_allocation_5.getStartTime() + dur);

        estimated_allocation_6.setStartTime(estimated_allocation_5.getEndTime() + r5.getDuration());
        estimated_allocation_6.setEndTime(estimated_allocation_6.getStartTime());
    }

    SECTION("Narrow time windows with budget")
    {
        op1->setTimeWindows({make_time_window(770, 20000)});
        op2->setTimeWindows({make_time_window(2140, 20000)});
        op3->setTimeWindows({make_time_window(3510, 20000)});
        op4->setTimeWindows({make_time_window(4880, 20000)});

        estimated_allocation_1.setStartTime(TimePoint(400));
        estimated_allocation_1.setEndTime(estimated_allocation_1.getStartTime());

        estimated_allocation_2.setStartTime(TimePoint(1070));
        estimated_allocation_2.setEndTime(estimated_allocation_2.getStartTime() + dur);

        estimated_allocation_3.setStartTime(TimePoint(2340));
        estimated_allocation_3.setEndTime(estimated_allocation_3.getStartTime() + dur);

        estimated_allocation_4.setStartTime(TimePoint(3610));
        estimated_allocation_4.setEndTime(estimated_allocation_4.getStartTime() + dur);

        estimated_allocation_5.setStartTime(TimePoint(4880));
        estimated_allocation_5.setEndTime(estimated_allocation_5.getStartTime() + dur);

        estimated_allocation_6.setStartTime(estimated_allocation_5.getEndTime() + r5.getDuration());
        estimated_allocation_6.setEndTime(estimated_allocation_6.getStartTime());
    }

    SECTION("Narrow time windows with budget gap")
    {
        op1->setTimeWindows({make_time_window(770, 20000)});
        op2->setTimeWindows({make_time_window(2140, 2740)}); // <== This stop can't be shifted to compensate waiting due to its time window
        op3->setTimeWindows({make_time_window(3510, 20000)});
        op4->setTimeWindows({make_time_window(4880, 20000)});

        estimated_allocation_1.setStartTime(TimePoint(200));
        estimated_allocation_1.setEndTime(estimated_allocation_1.getStartTime());

        estimated_allocation_2.setStartTime(TimePoint(870));
        estimated_allocation_2.setEndTime(estimated_allocation_2.getStartTime() + dur);

        estimated_allocation_3.setStartTime(TimePoint(2140));
        estimated_allocation_3.setEndTime(estimated_allocation_3.getStartTime() + dur);

        estimated_allocation_4.setStartTime(TimePoint(3510));
        estimated_allocation_4.setEndTime(estimated_allocation_4.getStartTime() + dur);

        estimated_allocation_5.setStartTime(TimePoint(4880));
        estimated_allocation_5.setEndTime(estimated_allocation_5.getStartTime() + dur);

        estimated_allocation_6.setStartTime(estimated_allocation_5.getEndTime() + r5.getDuration());
        estimated_allocation_6.setEndTime(estimated_allocation_6.getStartTime());
    }

    Stop *s1 = r->getStartStop();
    Stop *s2 = r->allocateWorkOperation(op1, 0);
    Stop *s3 = r->allocateWorkOperation(op2, 1);
    Stop *s4 = r->allocateWorkOperation(op3, 2);
    Stop *s5 = r->allocateWorkOperation(op4, 3);
    Stop *s6 = r->getEndStop();

    CAPTURE(s1->getAllocationTime());
    CAPTURE(s2->getAllocationTime());
    CAPTURE(s3->getAllocationTime());
    CAPTURE(s4->getAllocationTime());
    CAPTURE(s5->getAllocationTime());
    CAPTURE(s6->getAllocationTime());

    REQUIRE(s1->getAllocationTime() == estimated_allocation_1);
    REQUIRE(s2->getAllocationTime() == estimated_allocation_2);
    REQUIRE(s3->getAllocationTime() == estimated_allocation_3);
    REQUIRE(s4->getAllocationTime() == estimated_allocation_4);
    REQUIRE(s5->getAllocationTime() == estimated_allocation_5);
    REQUIRE(s6->getAllocationTime() == estimated_allocation_6);

    sm->destroyScene(s);
}