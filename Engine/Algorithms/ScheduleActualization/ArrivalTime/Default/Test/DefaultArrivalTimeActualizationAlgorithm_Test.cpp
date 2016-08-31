#include <catch.hpp>

#include <Engine/Engine/Engine.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>
#include <Engine/Concepts/Test/MakeLocation.h>
#include <Engine/Concepts/Test/MakeTimeWindow.h>
#include <Engine/Concepts/Test/ConceptStreamOperators.h>
#include <Engine/Utils/Units/DurationUnits.h>
#include <Engine/Algorithms/ScheduleActualization/Route/Default/DefaultRouteActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/Duration/Default/DefaultDurationActualizationAlgorithm.h>

TEST_CASE("ScheduleActualizers - StopArrivalTimeActualizationAlgorithm", "[integration][schedule_actualizers][!mayfail]") // Temporary may fail (different crowfly routing service results on different platforms)
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

    Scene* s = sm->createScene(*scene_context);

    Duration dur = Units::minutes(10);

    Operation* op1 = scene_context->createFreeOperation(loc1);
    op1->setDuration(dur);

    Operation* op2 = scene_context->createFreeOperation(loc2);
    op2->setDuration(dur);

    Operation* op3 = scene_context->createFreeOperation(loc3);
    op3->setDuration(dur);

    Operation* op4 = scene_context->createFreeOperation(loc4);
    op4->setDuration(dur);

    Performer* performer = scene_context->createPerformer();
	performer->setDepot(scene_context->createDepot(start_location));
    Vehicle* vehicle = scene_context->createVehicle();

    Schedule* schedule = s->createSchedule(*performer);

    Run* r = *schedule->createRun(schedule->getRuns().end(), start_location, end_location);
    r->setVehicle(vehicle);

    Route r1 = routing_service.calculateRoute(start_location.getSite(), loc1.getSite(), vehicle->getRoutingProfile());
    Route r2 = routing_service.calculateRoute(loc1.getSite(), loc2.getSite(), vehicle->getRoutingProfile());
    Route r3 = routing_service.calculateRoute(loc2.getSite(), loc3.getSite(), vehicle->getRoutingProfile());
    Route r4 = routing_service.calculateRoute(loc3.getSite(), loc4.getSite(), vehicle->getRoutingProfile());
    Route r5 = routing_service.calculateRoute(loc4.getSite(), end_location.getSite(), vehicle->getRoutingProfile());

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
        op1->constraints().timeWindows().set({make_time_window(770, 1370)});
        op2->constraints().timeWindows().set({make_time_window(2140, 2740)});
        op3->constraints().timeWindows().set({make_time_window(3510, 4110)});
        op4->constraints().timeWindows().set({make_time_window(4880, 5480)});

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
        op1->constraints().timeWindows().set({make_time_window(770, 20000)});
        op2->constraints().timeWindows().set({make_time_window(2140, 20000)});
        op3->constraints().timeWindows().set({make_time_window(3510, 20000)});
        op4->constraints().timeWindows().set({make_time_window(4880, 20000)});

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
        op1->constraints().timeWindows().set({make_time_window(770, 20000)});
        op2->constraints().timeWindows().set({make_time_window(2140, 2740)}); // <== This stop can't be shifted to compensate waiting due to its time window
        op3->constraints().timeWindows().set({make_time_window(3510, 20000)});
        op4->constraints().timeWindows().set({make_time_window(4880, 20000)});

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
    Stop *s2 = *r->createWorkStop(r->getWorkStops().end(), op1);
    Stop *s3 = *r->createWorkStop(r->getWorkStops().end(), op2);
    Stop *s4 = *r->createWorkStop(r->getWorkStops().end(), op3);
    Stop *s5 = *r->createWorkStop(r->getWorkStops().end(), op4);
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