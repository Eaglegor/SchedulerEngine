#include <catch.hpp>

#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <Persistence/SceneLoaders/JSONSceneLoader/JSONSceneLoader.h>
#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Units/DurationUnits.h>
#include <Engine/Utils/Units/SpeedUnits.h>
#include <Engine/Utils/Collections/Algorithms.h>

TEST_CASE("Persistence - SceneLoaders - JSONSceneLoader", "[integration][functional][persistence]")
{
	boost::property_tree::ptree props;
	using namespace Scheduler;

	CrowFlyRoutingService rs;

	Engine engine;

	SceneManager* sm = engine.getSceneManager();

	std::ifstream ifile;
	ifile.open("TestData/JSONSceneLoader_Test/TestScene1.json");
	REQUIRE(ifile.is_open());

	JSONSceneLoader loader(sm, &rs);
	Scene* scene = loader.loadScene(ifile);

	REQUIRE(scene);

	Site DC_Site(Coordinate::createFromFloat(34.567), Coordinate::createFromFloat(23.567));
	Site Order1_Site(Coordinate::createFromFloat(23.65), Coordinate::createFromFloat(23.64));
	Site Order2_Pickup(Coordinate::createFromFloat(25.454), Coordinate::createFromFloat(23.5353));
	Site Order2_Drop(Coordinate::createFromFloat(43.64), Coordinate::createFromFloat(54.75));
	Site Order3_Site(Coordinate::createFromFloat(54.23), Coordinate::createFromFloat(23.64));
	Site FreeOperation1_Site(Coordinate::createFromFloat(23.44), Coordinate::createFromFloat(23.53));

	INFO("Checking free operations")
	{
		REQUIRE(scene->getContext().getFreeOperations().size() == 1);

		Operation *FreeOperation1 = scene->getContext().getFreeOperations()[0];
		REQUIRE(strcmp(FreeOperation1->getName(), "FreeOperation1") == 0);
		REQUIRE(FreeOperation1->getLocation().getSite() == FreeOperation1_Site);
		REQUIRE(FreeOperation1->getDuration() == Units::minutes(10));
		REQUIRE(FreeOperation1->getOrder() == nullptr);
		REQUIRE(FreeOperation1->constraints().demand().get() == Capacity(0, 0, 0, 0));
		REQUIRE(FreeOperation1->constraints().timeWindows().get().size() == 1);
		REQUIRE(FreeOperation1->constraints().timeWindows().get()[0].getStartTime() - Units::hours_minutes(12, 0) == TimePoint(0));
		REQUIRE(FreeOperation1->constraints().timeWindows().get()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
	}

	INFO("Checking order")
	{
		REQUIRE(scene->getContext().getOrders().size() == 3);

		Order* Order1 = scene->getContext().getOrders()[0];
		REQUIRE(strcmp(Order1->getName(), "Order1") == 0);
		//REQUIRE(Order1->getVehicleRequirements().size() == 2);
		//REQUIRE(std::contains_key(Order1->getVehicleRequirements(), sm->createAttribute("Attr1")));
		//REQUIRE(std::contains_key(Order1->getVehicleRequirements(), sm->createAttribute("Attr2")));
		//REQUIRE(Order1->getPerformerSkillsRequirements().empty());
		REQUIRE(Order1->getStartOperation() != nullptr);
		{
			Operation* Order1_start = Order1->getStartOperation();
			REQUIRE(strcmp(Order1_start->getName(), "Order1.pickup") == 0);
			REQUIRE(Order1_start->getLocation().getSite() == DC_Site);
			REQUIRE(Order1_start->getDuration() == Units::minutes(3));
			REQUIRE(Order1_start->getOrder() == Order1);
			REQUIRE(Order1_start->constraints().demand().get() == Capacity(1, 0, 0, 0));
			REQUIRE(Order1_start->constraints().timeWindows().get().size() == 1);
			REQUIRE(Order1_start->constraints().timeWindows().get()[0].getStartTime() - Units::hours_minutes(04, 0) == TimePoint(0));
			REQUIRE(Order1_start->constraints().timeWindows().get()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
		}
		REQUIRE(Order1->getWorkOperations().size() == 1);
		{
			Operation* Order1_work0 = Order1->getWorkOperations()[0];
			REQUIRE(strcmp(Order1_work0->getName(), "Order1.drop") == 0);
			REQUIRE(Order1_work0->getLocation().getSite() == Order1_Site);
			REQUIRE(Order1_work0->getDuration() == Units::minutes(10));
			REQUIRE(Order1_work0->getOrder() == Order1);
			REQUIRE(Order1_work0->constraints().demand().get() == Capacity(-1, 0, 0, 0));
			REQUIRE(Order1_work0->constraints().timeWindows().get().size() == 1);
			REQUIRE(Order1_work0->constraints().timeWindows().get()[0].getStartTime() - Units::hours_minutes(10, 0) == TimePoint(0));
			REQUIRE(Order1_work0->constraints().timeWindows().get()[0].getEndTime() - Units::hours_minutes(10, 30) == TimePoint(0));
		}
		REQUIRE(Order1->getEndOperation() == nullptr);

		Order* Order2 = scene->getContext().getOrders()[1];
		REQUIRE(strcmp(Order2->getName(), "Order2") == 0);
		//REQUIRE(Order2->getVehicleRequirements().empty());
		//REQUIRE(Order2->getPerformerSkillsRequirements().empty());
		REQUIRE(Order2->getStartOperation() == nullptr);
		REQUIRE(Order2->getWorkOperations().size() == 2);
		{
			Operation* Order2_work0 = Order2->getWorkOperations()[0];
			REQUIRE(strcmp(Order2_work0->getName(), "Order2.collection") == 0);
			REQUIRE(Order2_work0->getLocation().getSite() == Order2_Pickup);
			REQUIRE(Order2_work0->getDuration() == Units::minutes(10));
			REQUIRE(Order2_work0->getOrder() == Order2);
			REQUIRE(Order2_work0->constraints().demand().get() == Capacity(2, 0, 0, 0));
			REQUIRE(Order2_work0->constraints().timeWindows().get().size() == 1);
			REQUIRE(Order2_work0->constraints().timeWindows().get()[0].getStartTime() - Units::hours_minutes(10, 0) == TimePoint(0));
			REQUIRE(Order2_work0->constraints().timeWindows().get()[0].getEndTime() - Units::hours_minutes(12, 0) == TimePoint(0));

			Operation* Order2_work1 = Order2->getWorkOperations()[1];
			REQUIRE(strcmp(Order2_work1->getName(), "Order2.delivery") == 0);
			REQUIRE(Order2_work1->getLocation().getSite() == Order2_Drop);
			REQUIRE(Order2_work1->getDuration() == Units::minutes(10));
			REQUIRE(Order2_work1->getOrder() == Order2);
			REQUIRE(Order2_work1->constraints().demand().get() == Capacity(-2, 0, 0, 0));
			REQUIRE(Order2_work1->constraints().timeWindows().get().size() == 1);
			REQUIRE(Order2_work1->constraints().timeWindows().get()[0].getStartTime() - Units::hours_minutes(12, 0) == TimePoint(0));
			REQUIRE(Order2_work1->constraints().timeWindows().get()[0].getEndTime() - Units::hours_minutes(15, 0) == TimePoint(0));
		}
		REQUIRE(Order2->getEndOperation() == nullptr);

		Order* Order3 = scene->getContext().getOrders()[2];
		REQUIRE(strcmp(Order3->getName(), "Order3") == 0);
		//REQUIRE(Order3->getVehicleRequirements().empty());
		//REQUIRE(Order3->getPerformerSkillsRequirements().empty());
		REQUIRE(Order3->getStartOperation() == nullptr);
		REQUIRE(Order3->getWorkOperations().size() == 1);
		{
			Operation* Order3_work0 = Order3->getWorkOperations()[0];
			REQUIRE(strcmp(Order3_work0->getName(), "Order3.work") == 0);
			REQUIRE(Order3_work0->getLocation().getSite() == Order3_Site);
			REQUIRE(Order3_work0->getDuration() == Units::minutes(10));
			REQUIRE(Order3_work0->getOrder() == Order3);
			REQUIRE(Order3_work0->constraints().demand().get() == Capacity(0, 0, 0, 0));
			REQUIRE(Order3_work0->constraints().timeWindows().get().size() == 1);
			REQUIRE(Order3_work0->constraints().timeWindows().get()[0].getStartTime() - Units::hours_minutes(12, 0) == TimePoint(0));
			REQUIRE(Order3_work0->constraints().timeWindows().get()[0].getEndTime() - Units::hours_minutes(16, 0) == TimePoint(0));
		}
		REQUIRE(Order3->getEndOperation() == nullptr);
	}

	INFO("Checking performers")
	{
		REQUIRE(scene->getContext().getPerformers().size() == 1);

		Performer* Driver1 = scene->getContext().getPerformers()[0];
		REQUIRE(strcmp(Driver1->getName(), "Driver1") == 0);
		REQUIRE(Driver1->getSkills().size() == 1);
		REQUIRE(util::contains_key(Driver1->getSkills(), scene->getContext().getAttribute("Driving")));
		REQUIRE(Driver1->getDurationUnitCost() == Cost(0));
		REQUIRE(Driver1->getActivationCost() == Cost(0));
		REQUIRE(Driver1->constraints().availabilityWindows().get().size() == 1);
		REQUIRE(Driver1->constraints().availabilityWindows().get()[0].getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
		REQUIRE(Driver1->constraints().availabilityWindows().get()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
	}

	INFO("Checking vehicles")
	{
		REQUIRE(scene->getContext().getVehicles().size() == 1);

		Vehicle* Vehicle1 = scene->getContext().getVehicles()[0];
		REQUIRE(strcmp(Vehicle1->getName(), "Vehicle1") == 0);
		REQUIRE(Vehicle1->getAttributes().size() == 2);
		REQUIRE(util::contains_key(Vehicle1->getAttributes(), scene->getContext().getAttribute("Attr1")));
		REQUIRE(util::contains_key(Vehicle1->getAttributes(), scene->getContext().getAttribute("Attr2")));
		REQUIRE(Vehicle1->getDurationUnitCost() == Cost(0));
		REQUIRE(Vehicle1->getDistanceUnitCost() == Cost(1));
		REQUIRE(Vehicle1->getActivationCost() == Cost(0));
		REQUIRE(Vehicle1->constraints().capacity().get() == Capacity(3, 0, 0, 0));
		REQUIRE(Vehicle1->getRoutingProfile().getAverageSpeed() == Speed(Distance(60)));
		REQUIRE(Vehicle1->constraints().availabilityWindows().get().size() == 1);
		REQUIRE(Vehicle1->constraints().availabilityWindows().get()[0].getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
		REQUIRE(Vehicle1->constraints().availabilityWindows().get()[0].getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));
	}

	INFO("Checking schedules")
	{
		REQUIRE(scene->getSchedules().size() == 1);

		Schedule* schedule1 = scene->getSchedules()[0];
		REQUIRE(schedule1->getPerformer() == scene->getContext().getPerformers()[0]);
		//REQUIRE(schedule1->getDepotSite() == DC_Site);
		REQUIRE_FALSE(schedule1->constraints().shiftStartLocation().isSet());
		REQUIRE_FALSE(schedule1->constraints().shiftEndLocation().isSet());
		REQUIRE(schedule1->getShift().getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
		REQUIRE(schedule1->getShift().getEndTime() - Units::hours_minutes(18, 0) == TimePoint(0));

		REQUIRE(schedule1->getRuns().size() == 1);
		{
			Run* run1 = schedule1->getRuns()[0];
			REQUIRE(run1->getStartStop()->getLocation().getSite() == DC_Site);
			REQUIRE(run1->getEndStop()->getLocation().getSite() == DC_Site);
			REQUIRE(run1->getVehicle() == scene->getContext().getVehicles()[0]);
			REQUIRE(run1->getStartStop()->getOperations().empty());
			REQUIRE(run1->getWorkStops().size() == 3);
			{
				WorkStop* stop1 = *std::next(run1->getWorkStops().begin(), 0);
				REQUIRE(stop1->getOperation() == scene->getContext().getOrders()[0]->getWorkOperations()[0]);
				// Values after actualization don't match initial values in json
				//REQUIRE(stop1->getAllocationTime().getStartTime() - Units::hours_minutes(8, 0) == TimePoint(0));
				//REQUIRE(stop1->getAllocationTime().getEndTime() - Units::hours_minutes(8, 3) == TimePoint(0));

				WorkStop* stop2 = *std::next(run1->getWorkStops().begin(), 1);
				REQUIRE(stop2->getOperation() == scene->getContext().getOrders()[1]->getWorkOperations()[0]);
				// Values after actualization don't match initial values in json
				//REQUIRE(stop2->getAllocationTime().getStartTime() - Units::hours_minutes(9, 0) == TimePoint(0));
				//REQUIRE(stop2->getAllocationTime().getEndTime() - Units::hours_minutes(9, 10) == TimePoint(0));

				WorkStop* stop3 = *std::next(run1->getWorkStops().begin(), 2);
				REQUIRE(stop3->getOperation() == scene->getContext().getOrders()[1]->getWorkOperations()[1]);
				// Values after actualization don't match initial values in json
				//REQUIRE(stop3->getAllocationTime().getStartTime() - Units::hours_minutes(10, 0) == TimePoint(0));
				//REQUIRE(stop3->getAllocationTime().getEndTime() - Units::hours_minutes(11, 10) == TimePoint(0));
			}
			REQUIRE(run1->getEndStop()->getOperations().empty());
		}
	}

	ifile.close();
}
