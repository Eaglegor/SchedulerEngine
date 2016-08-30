#include <catch.hpp>

#include <Engine/Concepts/Test/MakeTimeWindow.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Location.h>
#include <string>

using namespace Scheduler;

TEST_CASE("SceneManager - SceneContext - Creation")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	
	SceneContext* scene_context = sm->createSceneContext();
	
	REQUIRE(scene_context != nullptr);
	REQUIRE(scene_context->getFreeOperations().empty());
	REQUIRE(scene_context->getOrders().empty());
	REQUIRE(scene_context->getPerformers().empty());
	REQUIRE(scene_context->getVehicles().empty());
	REQUIRE(scene_context->getLocations().empty());
	REQUIRE(scene_context->getDepots().empty());
	
	const SceneContext* cscene_context = scene_context;
	
	REQUIRE(&cscene_context->getFreeOperations() == &scene_context->getFreeOperations());
	REQUIRE(&cscene_context->getOrders() == &scene_context->getOrders());
	REQUIRE(&cscene_context->getPerformers() == &scene_context->getPerformers());
	REQUIRE(&cscene_context->getVehicles() == &scene_context->getVehicles());
	REQUIRE(&cscene_context->getLocations() == &scene_context->getLocations());
	REQUIRE(&cscene_context->getDepots() == &scene_context->getDepots());
	REQUIRE(&cscene_context->constraints() == &scene_context->constraints());
}

TEST_CASE("SceneManager - SceneContext - Location")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	Site site(Coordinate(5), Coordinate(5));
	
	Location* location = scene_context->createLocation(site);
	
	REQUIRE(location != nullptr);
	REQUIRE(std::string(location->getName()) == "");
	REQUIRE(location->getSite() == site);
	
	location->setName("Location1");
	
	REQUIRE(std::string(location->getName()) == "Location1");
}

TEST_CASE("SceneManager - SceneContext - Attribute")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	const Attribute* attr1 = scene_context->createAttribute("Attr1");
	const Attribute* attr2 = scene_context->createAttribute("Attr2");
	const Attribute* attr3 = scene_context->createAttribute("Attr1");
	
	REQUIRE(attr1 != nullptr);
	REQUIRE(attr2 != nullptr);
	REQUIRE(attr3 != nullptr);
	REQUIRE(attr1 == attr3);
	REQUIRE(attr1 != attr2);
	REQUIRE(attr2 != attr3);
}

TEST_CASE("SceneManager - SceneContext - FreeOperation")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	Site site(Coordinate(5), Coordinate(5));
	const Location& location = *scene_context->createLocation(site);
	
	Operation* fop = scene_context->createFreeOperation(location);
	
	REQUIRE(fop != nullptr);
	REQUIRE(fop->getLocation() == location);
	REQUIRE(std::string(fop->getName()) == "");
	REQUIRE(fop->getDuration() == Duration(0));
	REQUIRE(fop->getOrder() == nullptr);
	
	fop->setName("Operation1");
	fop->setDuration(Duration(6));
	
	REQUIRE(std::string(fop->getName()) == "Operation1");
	REQUIRE(fop->getDuration() == Duration(6));
	
	const Operation* cfop = fop;
	REQUIRE(&fop->constraints() == &cfop->constraints());
}

TEST_CASE("SceneManager - SceneContext - FreeOperation - Constraints")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	Site site(Coordinate(5), Coordinate(5));
	const Location& location = *scene_context->createLocation(site);
	
	Operation* fop = scene_context->createFreeOperation(location);

	REQUIRE_FALSE(fop->constraints().timeWindows().isSet());
	REQUIRE_FALSE(fop->constraints().demand().isSet());
	REQUIRE_FALSE(fop->constraints().vehicleAttributesRequirements().isSet());
	REQUIRE_FALSE(fop->constraints().performerSkillsRequirements().isSet());

	const Attribute* truck = scene_context->createAttribute("Truck");
	const Attribute* light = scene_context->createAttribute("Light");
	const Attribute* electric = scene_context->createAttribute("Electric");
	const Attribute* weights = scene_context->createAttribute("Weights");
	
	fop->constraints().timeWindows().set({make_time_window(0, 3), make_time_window(4, 6)});
	fop->constraints().demand().set(Capacity(0, 2, 3, 4));
	fop->constraints().vehicleAttributesRequirements().set({truck, light});
	fop->constraints().performerSkillsRequirements().set({electric, weights});
	
	REQUIRE(fop->constraints().timeWindows().isSet());
	REQUIRE(fop->constraints().timeWindows().get().size() == 2);
	REQUIRE(fop->constraints().timeWindows().get()[0] == make_time_window(0, 3));
	REQUIRE(fop->constraints().timeWindows().get()[1] == make_time_window(4, 6));

	REQUIRE(fop->constraints().demand().isSet());
	REQUIRE(fop->constraints().demand().get() == Capacity(0, 2, 3, 4));

	REQUIRE(fop->constraints().vehicleAttributesRequirements().isSet());
	REQUIRE(fop->constraints().vehicleAttributesRequirements().get().size() == 2);
	REQUIRE(fop->constraints().vehicleAttributesRequirements().get()[0] == truck);
	REQUIRE(fop->constraints().vehicleAttributesRequirements().get()[1] == light);


	REQUIRE(fop->constraints().performerSkillsRequirements().isSet());
	REQUIRE(fop->constraints().performerSkillsRequirements().get().size() == 2);
	REQUIRE(fop->constraints().performerSkillsRequirements().get()[0] == electric);
	REQUIRE(fop->constraints().performerSkillsRequirements().get()[1] == weights);
	
	Operation* fop2 = scene_context->createFreeOperation(location);
	
	REQUIRE(fop2 != nullptr);
	
	fop2->constraints() = fop->constraints();
	
	REQUIRE(fop2->constraints().timeWindows().isSet());
	REQUIRE(fop2->constraints().timeWindows().get().size() == 2);
	REQUIRE(fop2->constraints().timeWindows().get()[0] == make_time_window(0, 3));
	REQUIRE(fop2->constraints().timeWindows().get()[1] == make_time_window(4, 6));

	REQUIRE(fop2->constraints().demand().isSet());
	REQUIRE(fop2->constraints().demand().get() == Capacity(0, 2, 3, 4));

	REQUIRE(fop2->constraints().vehicleAttributesRequirements().isSet());
	REQUIRE(fop2->constraints().vehicleAttributesRequirements().get().size() == 2);
	REQUIRE(fop2->constraints().vehicleAttributesRequirements().get()[0] == truck);
	REQUIRE(fop2->constraints().vehicleAttributesRequirements().get()[1] == light);

	REQUIRE(fop2->constraints().performerSkillsRequirements().isSet());
	REQUIRE(fop2->constraints().performerSkillsRequirements().get().size() == 2);
	REQUIRE(fop2->constraints().performerSkillsRequirements().get()[0] == electric);
	REQUIRE(fop2->constraints().performerSkillsRequirements().get()[1] == weights);
	
}

TEST_CASE("SceneManager - SceneContext - Order")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	
	Order* order = scene_context->createOrder();
	
	REQUIRE(order != nullptr);
	REQUIRE(std::string(order->getName()) == "");
	REQUIRE(order->getStartOperation() == nullptr);
	REQUIRE(order->getWorkOperations().empty());
	REQUIRE(order->getEndOperation() == nullptr);
	
	order->setName("Order1");
	
	REQUIRE(std::string(order->getName()) == "Order1");
	
	const Order* corder = order;
	
	REQUIRE(&corder->constraints() == &order->constraints());
}

TEST_CASE("SceneManager - SceneContext - Order - Operations")
{
	Engine engine;
	SceneManager* sm = engine.getSceneManager();
	SceneContext* scene_context = sm->createSceneContext();
	Order* order = scene_context->createOrder();
	
	Site start(Coordinate(5), Coordinate(5));
	Site work1(Coordinate(1), Coordinate(5));
	Site work2(Coordinate(5), Coordinate(1));
	Site end(Coordinate(1), Coordinate(1));
	
	const Location& start_location = *scene_context->createLocation(start);
	const Location& work1_location = *scene_context->createLocation(work1);
	const Location& work2_location = *scene_context->createLocation(work2);
	const Location& end_location = *scene_context->createLocation(end);
	
	Operation* start_operation = order->createStartOperation(start_location);
	
	REQUIRE(start_operation->getLocation() == start_location);
	REQUIRE(order->getStartOperation() == start_operation);
	REQUIRE(order->getWorkOperations().empty());
	REQUIRE(order->getEndOperation() == nullptr);
	
	Operation* start_operation2 = order->createStartOperation(start_location);
	
	REQUIRE(order->getStartOperation() == start_operation2);
	REQUIRE(order->getWorkOperations().empty());
	REQUIRE(order->getEndOperation() == nullptr);
	
	Operation* end_operation = order->createEndOperation(end_location);
	
	REQUIRE(end_operation->getLocation() == end_location);	
	REQUIRE(order->getStartOperation() == start_operation2);
	REQUIRE(order->getWorkOperations().empty());
	REQUIRE(order->getEndOperation() == end_operation);
	
	Operation* end_operation2 = order->createEndOperation(end_location);

	REQUIRE(order->getStartOperation() == start_operation2);
	REQUIRE(order->getWorkOperations().empty());
	REQUIRE(order->getEndOperation() == end_operation2);
	
	Operation* work_operation1 = order->createWorkOperation(work1_location);	
	
	REQUIRE(work_operation1->getLocation() == work1_location);	
	REQUIRE(order->getStartOperation() == start_operation2);
	REQUIRE(order->getWorkOperations().size() == 1);
	REQUIRE(order->getWorkOperations()[0] == work_operation1);
	REQUIRE(order->getEndOperation() == end_operation2);
	
	Operation* work_operation2 = order->createWorkOperation(work2_location);	
	
	REQUIRE(work_operation2->getLocation() == work2_location);	
	REQUIRE(order->getStartOperation() == start_operation2);
	REQUIRE(order->getWorkOperations().size() == 2);
	REQUIRE(order->getWorkOperations()[0] == work_operation1);
	REQUIRE(order->getWorkOperations()[1] == work_operation2);
	REQUIRE(order->getEndOperation() == end_operation2);
}

