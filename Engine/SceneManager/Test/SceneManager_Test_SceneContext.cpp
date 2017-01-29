#include <catch.hpp>

#include <Engine/Concepts/Test/MakeTimeWindow.h>
#include <Engine/Core/Engine.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneManager/Utils/StreamIO.h>
#include <string>

using namespace Scheduler;

TEST_CASE("SceneManager - SceneContext - Creation")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	
	SceneContext& scene_context = sm.createSceneContext();
	
	REQUIRE(scene_context.getFreeOperations().empty());
	REQUIRE(scene_context.getOrders().empty());
	REQUIRE(scene_context.getPerformers().empty());
	REQUIRE(scene_context.getVehicles().empty());
	REQUIRE(scene_context.getLocations().empty());
	REQUIRE(scene_context.getDepots().empty());
}

TEST_CASE("SceneManager - SceneContext - Location")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Site site(Coordinate(5), Coordinate(5));
	
	Location& location = scene_context.createLocation(site);
	
	REQUIRE(location.getName() == "");
	REQUIRE(location.getSite() == site);
	
	location.setName("Location1");
	
	REQUIRE(location.getName() == "Location1");
}

TEST_CASE("SceneManager - SceneContext - Attribute")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	const Attribute& attr1 = scene_context.createAttribute("Attr1");
	const Attribute& attr2 = scene_context.createAttribute("Attr2");
	const Attribute& attr3 = scene_context.createAttribute("Attr1");

	REQUIRE(attr1 == attr3);
	REQUIRE(attr1 != attr2);
	REQUIRE(attr2 != attr3);
}

TEST_CASE("SceneManager - SceneContext - FreeOperation")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Site site(Coordinate(5), Coordinate(5));
	const Location& location = scene_context.createLocation(site);
	
	Operation& fop = scene_context.createFreeOperation(location);
	
	REQUIRE(fop.getLocation() == location);
	REQUIRE(fop.getName() == "");
	REQUIRE(fop.getDuration() == Duration(0));
	REQUIRE(!fop.getOrder());
	
	fop.setName("Operation1");
	fop.setDuration(Duration(6));
	
	REQUIRE(fop.getName() == "Operation1");
	REQUIRE(fop.getDuration() == Duration(6));
}

TEST_CASE("SceneManager - SceneContext - FreeOperation - Constraints")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Site site(Coordinate(5), Coordinate(5));
	const Location& location = scene_context.createLocation(site);
	
	Operation& fop = scene_context.createFreeOperation(location);

	REQUIRE_FALSE(fop.constraints().timeWindows().isSet());
	REQUIRE_FALSE(fop.constraints().demand().isSet());
	REQUIRE_FALSE(fop.constraints().vehicleAttributesRequirements().isSet());
	REQUIRE_FALSE(fop.constraints().performerSkillsRequirements().isSet());

	const Attribute& truck = scene_context.createAttribute("Truck");
	const Attribute& light = scene_context.createAttribute("Light");
	const Attribute& electric = scene_context.createAttribute("Electric");
	const Attribute& weights = scene_context.createAttribute("Weights");
	
	fop.constraints().timeWindows().set({make_time_window(0, 3), make_time_window(4, 6)});
	fop.constraints().demand().set(Capacity(0, 2, 3, 4));
	fop.constraints().vehicleAttributesRequirements().set({truck, light});
	fop.constraints().performerSkillsRequirements().set({electric, weights});
	
	REQUIRE(fop.constraints().timeWindows().isSet());
	REQUIRE(fop.constraints().timeWindows().get().size() == 2);
	REQUIRE(fop.constraints().timeWindows().get()[0] == make_time_window(0, 3));
	REQUIRE(fop.constraints().timeWindows().get()[1] == make_time_window(4, 6));

	REQUIRE(fop.constraints().demand().isSet());
	REQUIRE(fop.constraints().demand().get() == Capacity(0, 2, 3, 4));

	REQUIRE(fop.constraints().vehicleAttributesRequirements().isSet());
	REQUIRE(fop.constraints().vehicleAttributesRequirements().get().size() == 2);
	REQUIRE(fop.constraints().vehicleAttributesRequirements().get()[0] == truck);
	REQUIRE(fop.constraints().vehicleAttributesRequirements().get()[1] == light);

	REQUIRE(fop.constraints().performerSkillsRequirements().isSet());
	REQUIRE(fop.constraints().performerSkillsRequirements().get().size() == 2);
	REQUIRE(fop.constraints().performerSkillsRequirements().get()[0] == electric);
	REQUIRE(fop.constraints().performerSkillsRequirements().get()[1] == weights);
	
	Operation& fop2 = scene_context.createFreeOperation(location);
	
	fop2.constraints() = fop.constraints();
	
	REQUIRE(fop2.constraints().timeWindows().isSet());
	REQUIRE(fop2.constraints().timeWindows().get().size() == 2);
	REQUIRE(fop2.constraints().timeWindows().get()[0] == make_time_window(0, 3));
	REQUIRE(fop2.constraints().timeWindows().get()[1] == make_time_window(4, 6));

	REQUIRE(fop2.constraints().demand().isSet());
	REQUIRE(fop2.constraints().demand().get() == Capacity(0, 2, 3, 4));

	REQUIRE(fop2.constraints().vehicleAttributesRequirements().isSet());
	REQUIRE(fop2.constraints().vehicleAttributesRequirements().get().size() == 2);
	REQUIRE(fop2.constraints().vehicleAttributesRequirements().get()[0] == truck);
	REQUIRE(fop2.constraints().vehicleAttributesRequirements().get()[1] == light);

	REQUIRE(fop2.constraints().performerSkillsRequirements().isSet());
	REQUIRE(fop2.constraints().performerSkillsRequirements().get().size() == 2);
	REQUIRE(fop2.constraints().performerSkillsRequirements().get()[0] == electric);
	REQUIRE(fop2.constraints().performerSkillsRequirements().get()[1] == weights);
	
}

TEST_CASE("SceneManager - SceneContext - Order")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Order& order = scene_context.createOrder();
	
	REQUIRE(order.getName() == "");
	REQUIRE(!order.getStartOperation());
	REQUIRE(!order.getWorkOperation());
	REQUIRE(!order.getEndOperation());
	
	order.setName("Order1");
	
	REQUIRE(order.getName() == "Order1");
}

TEST_CASE("SceneManager - SceneContext - Order - Operations")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	Order& order = scene_context.createOrder();
	
	Site start(Coordinate(5), Coordinate(5));
	Site work1(Coordinate(1), Coordinate(5));
	Site work2(Coordinate(5), Coordinate(1));
	Site end(Coordinate(1), Coordinate(1));
	
	const Location& start_location = scene_context.createLocation(start);
	const Location& work1_location = scene_context.createLocation(work1);
	const Location& work2_location = scene_context.createLocation(work2);
	const Location& end_location = scene_context.createLocation(end);
	
	Operation& start_operation = order.createStartOperation(start_location);
	
	REQUIRE(start_operation.getLocation() == start_location);
	REQUIRE(order.getStartOperation().get() == start_operation);
	REQUIRE(!order.getWorkOperation());
	REQUIRE(!order.getEndOperation());
	
	Operation& start_operation2 = order.createStartOperation(start_location);
	
	REQUIRE(start_operation2.getLocation() == start_location);
	REQUIRE(order.getStartOperation().get() == start_operation2);
	REQUIRE(!order.getWorkOperation());
	REQUIRE(!order.getEndOperation());
	
	Operation& end_operation = order.createEndOperation(end_location);
	
	REQUIRE(end_operation.getLocation() == end_location);	
	REQUIRE(order.getStartOperation().get() == start_operation2);
	REQUIRE(!order.getWorkOperation());
	REQUIRE(order.getEndOperation().get() == end_operation);
	
	Operation& end_operation2 = order.createEndOperation(end_location);

	REQUIRE(end_operation2.getLocation() == end_location);	
	REQUIRE(order.getStartOperation().get() == start_operation2);
	REQUIRE(!order.getWorkOperation());
	REQUIRE(order.getEndOperation().get() == end_operation2);
	
	Operation& work_operation1 = order.createWorkOperation(work1_location);	
	
	REQUIRE(work_operation1.getLocation() == work1_location);	
	REQUIRE(order.getStartOperation().get() == start_operation2);
	REQUIRE(order.getWorkOperation().get() == work_operation1);
	REQUIRE(order.getEndOperation().get() == end_operation2);
	
	Operation& work_operation2 = order.createWorkOperation(work2_location);	
	
	REQUIRE(work_operation2.getLocation() == work2_location);	
	REQUIRE(order.getStartOperation().get() == start_operation2);
	REQUIRE(order.getWorkOperation().get() == work_operation2);
	REQUIRE(order.getEndOperation().get() == end_operation2);
}

TEST_CASE("SceneManager - SceneContext - Depot")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Site depot_site(Coordinate(5), Coordinate(5));
	const Location& location = scene_context.createLocation(depot_site);
	
	Depot& depot = scene_context.createDepot(location);

	REQUIRE(depot.getLocation() == location);
	REQUIRE(depot.getName() == "");
	
	depot.setName("Depot1");
	
	REQUIRE(depot.getName() == "Depot1");
}

TEST_CASE("SceneManager - SceneContext - Performer")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Site depot_site(Coordinate(5), Coordinate(5));
	const Location& location = scene_context.createLocation(depot_site);
	
	Depot& depot = scene_context.createDepot(location);
	
	Performer& performer = scene_context.createPerformer(depot);
	
	REQUIRE(performer.getName() == "");
	REQUIRE(performer.getDurationUnitCost() == Cost(0));
	REQUIRE(performer.getActivationCost() == Cost(0));
	REQUIRE(performer.getSkills().empty());
	REQUIRE(performer.getDepot() == depot);
	
	const Attribute& skill = scene_context.createAttribute("skill");
	
	performer.setName("Performer1");
	performer.setDurationUnitCost(Cost(1));
	performer.setActivationCost(Cost(2));
	performer.addSkill(skill);
	
	REQUIRE(performer.getName() == "Performer1");
	REQUIRE(performer.getDurationUnitCost() == Cost(1));
	REQUIRE(performer.getActivationCost() == Cost(2));
	REQUIRE(performer.getSkills().size() == 1);
	REQUIRE(performer.getSkills().begin()->get() == skill);
	REQUIRE(performer.getDepot() == depot);
}

TEST_CASE("SceneManager - SceneContext - Performer - Constraints")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Performer& performer = scene_context.createPerformer();
	
	REQUIRE_FALSE(performer.constraints().availabilityWindows().isSet());
	
	performer.constraints().availabilityWindows().set({make_time_window(0, 15)});
	
	REQUIRE(performer.constraints().availabilityWindows().isSet());
	REQUIRE(performer.constraints().availabilityWindows().get().size() == 1);
	REQUIRE(performer.constraints().availabilityWindows().get()[0] == make_time_window(0, 15));
}

TEST_CASE("SceneManager - SceneContext - Vehicle")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Site depot_site(Coordinate(5), Coordinate(5));
	const Location& location = scene_context.createLocation(depot_site);
	
	Depot& depot = scene_context.createDepot(location);
	
	Vehicle& vehicle = scene_context.createVehicle(depot);
	
	REQUIRE(vehicle.getName() == "");
	REQUIRE(vehicle.getDurationUnitCost() == Cost(0));
	REQUIRE(vehicle.getDistanceUnitCost() == Cost(0));
	REQUIRE(vehicle.getActivationCost() == Cost(0));
	REQUIRE(vehicle.getAttributes().empty());
	REQUIRE(vehicle.getDepot() == depot);
	REQUIRE(vehicle.getRoutingProfile() == RoutingProfile());
	
	const Attribute& truck = scene_context.createAttribute("truck");
	
	vehicle.setName("Vehicle1");
	vehicle.setDurationUnitCost(Cost(1));
	vehicle.setDistanceUnitCost(Cost(2));
	vehicle.setActivationCost(Cost(3));
	vehicle.addAttribute(truck);
	
	RoutingProfile rp;
	rp.setAverageSpeed(Speed(Distance(5)));
	vehicle.setRoutingProfile(rp);
	
	REQUIRE(vehicle.getName() == "Vehicle1");
	REQUIRE(vehicle.getDurationUnitCost() == Cost(1));
	REQUIRE(vehicle.getDistanceUnitCost() == Cost(2));
	REQUIRE(vehicle.getActivationCost() == Cost(3));
	REQUIRE(vehicle.getAttributes().size() == 1);
	REQUIRE(vehicle.getAttributes().begin()->get() == truck);
	REQUIRE(vehicle.getDepot() == depot);
	REQUIRE(vehicle.getRoutingProfile() == rp);
}

TEST_CASE("SceneManager - SceneContext - Vehicle - Constraints")
{
	Engine engine;
	SceneManager& sm = engine.getSceneManager();
	SceneContext& scene_context = sm.createSceneContext();
	
	Vehicle& vehicle = scene_context.createVehicle();
	
	REQUIRE_FALSE(vehicle.constraints().capacity().isSet());
	REQUIRE_FALSE(vehicle.constraints().availabilityWindows().isSet());
	
	vehicle.constraints().capacity().set(Capacity(5));
	vehicle.constraints().availabilityWindows().set({make_time_window(0, 15)});
	
	REQUIRE(vehicle.constraints().capacity().isSet());
	REQUIRE(vehicle.constraints().capacity().get() == Capacity(5));
	REQUIRE(vehicle.constraints().availabilityWindows().isSet());
	REQUIRE(vehicle.constraints().availabilityWindows().get().size() == 1);
	REQUIRE(vehicle.constraints().availabilityWindows().get()[0] == make_time_window(0, 15));
}