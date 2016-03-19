#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/Utils/Units/DurationUnits.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include <Tests/Utils/ConceptStreamOperators.h>
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/EngineContext.h>

TEST_CASE("Persistence - SceneLoaders - JSONSceneLoader", "[integration][functional][persistence]")
{
	boost::property_tree::ptree props;
	using namespace Scheduler;

	CrowFlyRoutingService rs;

	EngineContext context;
	context.routing_service = &rs;

	Engine engine(context);

	SceneManager* sm = engine.getSceneManager();

	std::ifstream ifile;
	ifile.open("TestData/IFTest_JSONSceneLoader/TestScene1.json");
	REQUIRE(ifile.is_open());

	JSONSceneLoader loader(sm);
	Scene* scene = loader.loadScene(ifile);

	REQUIRE(scene);

	Location DC_Location(Coordinate(34.567), Coordinate(23.567));
	Location Order1_Location(Coordinate(23.65), Coordinate(23.64));
	Location Order2_Pickup(Coordinate(25.454), Coordinate(23.5353));
	Location Order2_Drop(Coordinate(43.64), Coordinate(54.75));
	Location Order3_Location(Coordinate(54.23), Coordinate(23.64));
	Location FreeOperation1_Location(Coordinate(23.44), Coordinate(23.53));

	INFO("Checking free operations")
	{
		REQUIRE(scene->getFreeOperations().size() == 1);

		Operation *FreeOperation1 = scene->getFreeOperations()[0];
		REQUIRE(strcmp(FreeOperation1->getName(), "FreeOperation1") == 0);
		REQUIRE(FreeOperation1->getLocation() == FreeOperation1_Location);
		REQUIRE(FreeOperation1->getDuration() == Units::minutes(10));
		REQUIRE(FreeOperation1->getOrder() == nullptr);
		REQUIRE(FreeOperation1->getLoad() == Capacity(0, 0, 0, 0));
		REQUIRE(FreeOperation1->getTimeWindows().size() == 1);
		REQUIRE(FreeOperation1->getTimeWindows()[0].getStartTime() - Units::hours_minutes(12, 0) == TimePoint(0));
		REQUIRE(FreeOperation1->getTimeWindows()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
	}

	INFO("Checking order")
	{
		REQUIRE(scene->getOrders().size() == 3);

		Order* Order1 = scene->getOrders()[0];
		REQUIRE(strcmp(Order1->getName(), "Order1") == 0);
		REQUIRE(Order1->getVehicleRequirements().size() == 2);
		REQUIRE(std::contains_key(Order1->getVehicleRequirements(), sm->createAttribute("Attr1")));
		REQUIRE(std::contains_key(Order1->getVehicleRequirements(), sm->createAttribute("Attr2")));
		REQUIRE(Order1->getPerformerSkillsRequirements().empty());
		REQUIRE(Order1->getStartOperation() != nullptr);
		{
			Operation* Order1_start = Order1->getStartOperation();
			REQUIRE(strcmp(Order1_start->getName(), "Order1.pickup") == 0);
			REQUIRE(Order1_start->getLocation() == DC_Location);
			REQUIRE(Order1_start->getDuration() == Units::minutes(3));
			REQUIRE(Order1_start->getOrder() == Order1);
			REQUIRE(Order1_start->getLoad() == Capacity(1, 0, 0, 0));
			REQUIRE(Order1_start->getTimeWindows().size() == 1);
			REQUIRE(Order1_start->getTimeWindows()[0].getStartTime() - Units::hours_minutes(04, 0) == TimePoint(0));
			REQUIRE(Order1_start->getTimeWindows()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
		}
		REQUIRE(Order1->getWorkOperations().size() == 1);
		{
			Operation* Order1_work0 = Order1->getWorkOperations()[0];
			REQUIRE(strcmp(Order1_work0->getName(), "Order1.drop") == 0);
			REQUIRE(Order1_work0->getLocation() == Order1_Location);
			REQUIRE(Order1_work0->getDuration() == Units::minutes(10));
			REQUIRE(Order1_work0->getOrder() == Order1);
			REQUIRE(Order1_work0->getLoad() == Capacity(-1, 0, 0, 0));
			REQUIRE(Order1_work0->getTimeWindows().size() == 1);
			REQUIRE(Order1_work0->getTimeWindows()[0].getStartTime() - Units::hours_minutes(10, 0) == TimePoint(0));
			REQUIRE(Order1_work0->getTimeWindows()[0].getEndTime() - Units::hours_minutes(10, 30) == TimePoint(0));
		}
		REQUIRE(Order1->getEndOperation() == nullptr);

		Order* Order2 = scene->getOrders()[1];
		REQUIRE(strcmp(Order2->getName(), "Order2") == 0);
		REQUIRE(Order2->getVehicleRequirements().empty());
		REQUIRE(Order2->getPerformerSkillsRequirements().empty());
		REQUIRE(Order2->getStartOperation() == nullptr);
		REQUIRE(Order2->getWorkOperations().size() == 2);
		{
			Operation* Order2_work0 = Order2->getWorkOperations()[0];
			REQUIRE(strcmp(Order2_work0->getName(), "Order2.collection") == 0);
			REQUIRE(Order2_work0->getLocation() == Order2_Pickup);
			REQUIRE(Order2_work0->getDuration() == Units::minutes(10));
			REQUIRE(Order2_work0->getOrder() == Order2);
			REQUIRE(Order2_work0->getLoad() == Capacity(2, 0, 0, 0));
			REQUIRE(Order2_work0->getTimeWindows().size() == 1);
			REQUIRE(Order2_work0->getTimeWindows()[0].getStartTime() - Units::hours_minutes(10, 0) == TimePoint(0));
			REQUIRE(Order2_work0->getTimeWindows()[0].getEndTime() - Units::hours_minutes(12, 0) == TimePoint(0));

			Operation* Order2_work1 = Order2->getWorkOperations()[1];
			REQUIRE(strcmp(Order2_work1->getName(), "Order2.delivery") == 0);
			REQUIRE(Order2_work1->getLocation() == Order2_Drop);
			REQUIRE(Order2_work1->getDuration() == Units::minutes(10));
			REQUIRE(Order2_work1->getOrder() == Order2);
			REQUIRE(Order2_work1->getLoad() == Capacity(-2, 0, 0, 0));
			REQUIRE(Order2_work1->getTimeWindows().size() == 1);
			REQUIRE(Order2_work1->getTimeWindows()[0].getStartTime() - Units::hours_minutes(12, 0) == TimePoint(0));
			REQUIRE(Order2_work1->getTimeWindows()[0].getEndTime() - Units::hours_minutes(15, 0) == TimePoint(0));
		}
		REQUIRE(Order2->getEndOperation() == nullptr);

		Order* Order3 = scene->getOrders()[2];
		REQUIRE(strcmp(Order3->getName(), "Order3") == 0);
		REQUIRE(Order3->getVehicleRequirements().empty());
		REQUIRE(Order3->getPerformerSkillsRequirements().empty());
		REQUIRE(Order3->getStartOperation() == nullptr);
		REQUIRE(Order3->getWorkOperations().size() == 1);
		{
			Operation* Order3_work0 = Order3->getWorkOperations()[0];
			REQUIRE(strcmp(Order3_work0->getName(), "Order3.work") == 0);
			REQUIRE(Order3_work0->getLocation() == Order3_Location);
			REQUIRE(Order3_work0->getDuration() == Units::minutes(10));
			REQUIRE(Order3_work0->getOrder() == Order3);
			REQUIRE(Order3_work0->getLoad() == Capacity(0, 0, 0, 0));
			REQUIRE(Order3_work0->getTimeWindows().size() == 1);
			REQUIRE(Order3_work0->getTimeWindows()[0].getStartTime() - Units::hours_minutes(12, 0) == TimePoint(0));
			REQUIRE(Order3_work0->getTimeWindows()[0].getEndTime() - Units::hours_minutes(16, 0) == TimePoint(0));
		}
		REQUIRE(Order3->getEndOperation() == nullptr);
	}

	INFO("Checking performers")
	{
		REQUIRE(scene->getPerformers().size() == 1);

		Performer* Driver1 = scene->getPerformers()[0];
		REQUIRE(strcmp(Driver1->getName(), "Driver1") == 0);
		REQUIRE(Driver1->getSkills().size() == 1);
		REQUIRE(std::contains_key(Driver1->getSkills(), sm->createAttribute("Driving")));
		REQUIRE(Driver1->getDurationUnitCost() == Cost(0));
		REQUIRE(Driver1->getActivationCost() == Cost(0));
		REQUIRE(Driver1->getAvailabilityWindows().size() == 1);
		REQUIRE(Driver1->getAvailabilityWindows()[0].getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
		REQUIRE(Driver1->getAvailabilityWindows()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
	}

	INFO("Checking vehicles")
	{
		REQUIRE(scene->getVehicles().size() == 1);

		Vehicle* Vehicle1 = scene->getVehicles()[0];
		REQUIRE(strcmp(Vehicle1->getName(), "Vehicle1") == 0);
		REQUIRE(Vehicle1->getAttributes().size() == 2);
		REQUIRE(std::contains_key(Vehicle1->getAttributes(), sm->createAttribute("Attr1")));
		REQUIRE(std::contains_key(Vehicle1->getAttributes(), sm->createAttribute("Attr2")));
		REQUIRE(Vehicle1->getDurationUnitCost() == Cost(0));
		REQUIRE(Vehicle1->getDistanceUnitCost() == Cost(1));
		REQUIRE(Vehicle1->getActivationCost() == Cost(0));
		REQUIRE(Vehicle1->getCapacity() == Capacity(3, 0, 0, 0));
		REQUIRE(Vehicle1->getRoutingProfile().getAverageSpeed() == Speed(Distance(60)));
		REQUIRE(Vehicle1->getAvailabilityWindows().size() == 1);
		REQUIRE(Vehicle1->getAvailabilityWindows()[0].getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
		REQUIRE(Vehicle1->getAvailabilityWindows()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
	}

	INFO("Checking schedules")
	{
		REQUIRE(scene->getSchedules().size() == 1);

		Schedule* schedule1 = scene->getSchedules()[0];
		REQUIRE(schedule1->getPerformer() == scene->getPerformers()[0]);
		REQUIRE(schedule1->getDepotLocation() == DC_Location);
		REQUIRE(schedule1->getShiftStartLocation() == DC_Location);
		REQUIRE(schedule1->getShiftEndLocation() == DC_Location);
		REQUIRE(schedule1->getShift().getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
		REQUIRE(schedule1->getShift().getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));

		REQUIRE(schedule1->getRuns().size() == 1);
		{
			Run* run1 = schedule1->getRuns()[0];
			REQUIRE(run1->getStartLocation() == DC_Location);
			REQUIRE(run1->getEndLocation() == DC_Location);
			REQUIRE(run1->getVehicle() == scene->getVehicles()[0]);
			REQUIRE(run1->getStartStop()->getOperations().empty());
			REQUIRE(run1->getWorkStops().size() == 3);
			{
				Stop* stop1 = run1->getWorkStops()[0];
				REQUIRE((*stop1->getOperations().begin()) == scene->getOrders()[0]->getWorkOperations()[0]);
				// Values after actualization don't match initial values in json
				//REQUIRE(stop1->getAllocationTime().getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
				//REQUIRE(stop1->getAllocationTime().getEndTime() - Units::hours_minutes(8, 3) == TimePoint(0));

				Stop* stop2 = run1->getWorkStops()[1];
				REQUIRE((*stop2->getOperations().begin()) == scene->getOrders()[1]->getWorkOperations()[0]);
				// Values after actualization don't match initial values in json
				//REQUIRE(stop2->getAllocationTime().getStartTime() - Units::hours_minutes(9, 0) == TimePoint(0));
				//REQUIRE(stop2->getAllocationTime().getEndTime() - Units::hours_minutes(9, 10) == TimePoint(0));

				Stop* stop3 = run1->getWorkStops()[2];
				REQUIRE((*stop3->getOperations().begin()) == scene->getOrders()[1]->getWorkOperations()[1]);
				// Values after actualization don't match initial values in json
				//REQUIRE(stop3->getAllocationTime().getStartTime() - Units::hours_minutes(10, 0) == TimePoint(0));
				//REQUIRE(stop3->getAllocationTime().getEndTime() - Units::hours_minutes(11, 10) == TimePoint(0));
			}
			REQUIRE(run1->getEndStop()->getOperations().empty());
		}
	}

	ifile.close();
}
