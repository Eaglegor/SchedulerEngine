#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include <Plugins/RoutingServices/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Basic/Capacity.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Concepts/Basic/RoutingProfile.h>
#include <Tests/Utils/Concepts/MakeLocation.h>
#include <Utils/Units/DurationUnits.h>
#include <Tests/Utils/Concepts/MakeTimeWindow.h>
#include <Utils/Collections/Algorithms.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <engine/SceneManager/ScheduleActualization/Algorithms/StopDurationActualizationAlgorithm.h>
#include <Engine/SceneManager/ScheduleActualization/Algorithms/StopArrivalTimeActualizationAlgorithm.h>

#include <Engine/SceneManager/RunVehicleBinding/PerformerAssignedVehicleBinder.h>
#include <Engine/SceneManager/RunVehicleBinding/PerformerAssignedVehicleBinderSettings.h>

TEST_CASE("SceneManager - RunVehicleSelectors", "[integration][scene_manager]")
{
    using namespace Scheduler;

    CrowFlyRoutingService routing_service;

    SceneManager sm;
    sm.setRoutingService(&routing_service);

    Scene* scene = sm.createScene();

    Vehicle* v1 = scene->createVehicle();
    Vehicle* v2 = scene->createVehicle();

    Performer* p1 = scene->createPerformer();
    Performer* p2 = scene->createPerformer();

    Location l = make_location(0, 0);

    SECTION("PerformerAssignedVehicleBinder")
    {
        PerformerAssignedVehicleBinderSettings settings;
        settings.assignments.emplace(p1, v2);
        settings.assignments.emplace(p2, v1);

        bool result = scene->createRunVehicleBinder<PerformerAssignedVehicleBinder>(settings);

        REQUIRE(result);

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