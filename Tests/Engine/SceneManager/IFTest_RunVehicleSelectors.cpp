#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Capacity.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Concepts/RoutingProfile.h>
#include <Tests/Utils/MakeLocation.h>
#include <Engine/Utils/Units/DurationUnits.h>
#include <Tests/Utils/MakeTimeWindow.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/Algorithms/ScheduleActualization/StopDuration/StopDurationActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/StopArrivalTime/StopArrivalTimeActualizationAlgorithm.h>

#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/EngineContext.h>

TEST_CASE("SceneManager - RunVehicleSelectors", "[integration][scene_manager]")
{
    using namespace Scheduler;

    CrowFlyRoutingService routing_service;

	EngineContext context;
	context.routing_service = &routing_service;
	Engine engine(context);

    SceneManager* sm = engine.getSceneManager();

    Scene* scene = sm->createScene();

    Vehicle* v1 = scene->createVehicle();
    Vehicle* v2 = scene->createVehicle();

    Performer* p1 = scene->createPerformer();
    Performer* p2 = scene->createPerformer();

    Location l = make_location(0, 0);

    SECTION("PerformerAssignedVehicleBinder")
    {
		PerformerAssignedVehicleBinder* binder = scene->createRunVehicleBinder<PerformerAssignedVehicleBinder>();

        REQUIRE(binder);

		binder->assign(p1, v2);
		binder->assign(p2, v1);

        Schedule* sch1 = scene->createSchedule(p1);
        Schedule* sch2 = scene->createSchedule(p2);

        Run* r11 = sch1->createRun(l, l);
        Run* r12 = sch1->createRun(l, l);

        Run* r21 = sch2->createRun(l, l);
        Run* r22 = sch2->createRun(l, l);

        REQUIRE(r11->getVehicle() == v2);
        REQUIRE(r12->getVehicle() == v2);

        REQUIRE(r21->getVehicle() == v1);
        REQUIRE(r22->getVehicle() == v1);
    }
}