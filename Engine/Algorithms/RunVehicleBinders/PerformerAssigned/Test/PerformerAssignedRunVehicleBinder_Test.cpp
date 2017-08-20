#include <catch.hpp>

#include <Engine/Algorithms/RunVehicleBinders/PerformerAssigned/PerformerAssignedVehicleBinder.h>
#include <Engine/Concepts/Test/MakeLocation.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Utils/StreamIO.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>


TEST_CASE("SceneManager - RunVehicleSelectors", "[integration][functional][scene_manager]")
{
	using namespace Scheduler;

	CrowFlyRoutingService routing_service;
	Engine engine;

	SceneManager& sm = engine.getSceneManager( );

	SceneContext& scene_context = sm.createSceneContext( );

	Vehicle& v1 = scene_context.createVehicle( );
	Vehicle& v2 = scene_context.createVehicle( );

	Performer& p1 = scene_context.createPerformer( );
	Performer& p2 = scene_context.createPerformer( );

	const Location& l = scene_context.createLocation(make_location(0, 0));

	AlgorithmsManager& am = engine.getAlgorithmsManager( );

	PerformerAssignedVehicleBinder& binder = am.createAlgorithm<PerformerAssignedVehicleBinder>( );

	binder.assign(p1, v2);
	binder.assign(p2, v1);

	Scene& scene = sm.createScene(scene_context);

	Schedule& sch1 = scene.createSchedule(p1);
	Schedule& sch2 = scene.createSchedule(p2);

	sch1.setRunVehicleBinder(binder);
	sch2.setRunVehicleBinder(binder);

	Run& r11 = *sch1.createRun(sch1.getRuns( ).end( ), l, l);
	Run& r12 = *sch1.createRun(sch1.getRuns( ).end( ), l, l);

	Run& r21 = *sch2.createRun(sch2.getRuns( ).end( ), l, l);
	Run& r22 = *sch2.createRun(sch2.getRuns( ).end( ), l, l);

	REQUIRE(r11.getVehicle( ) == v2);
	REQUIRE(r12.getVehicle( ) == v2);

	REQUIRE(r21.getVehicle( ) == v1);
	REQUIRE(r22.getVehicle( ) == v1);
}