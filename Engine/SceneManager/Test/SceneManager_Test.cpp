#include <catch.hpp>

#include <Services/Routing/CrowFlyRoutingService/CrowFlyRoutingService.h>
#include <Engine/Engine/Engine.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/RunBoundaryStop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Attribute.h>
#include <Engine/Concepts/Test/MakeLocation.h>
#include <Engine/Concepts/Test/MakeTimeWindow.h>
#include <Engine/Utils/Units/DurationUnits.h>
#include <Engine/Utils/Collections/Algorithms.h>

bool checkRoute(Scheduler::Stop* from, Scheduler::Stop* to, Scheduler::RoutingService* routing_service, Scheduler::Vehicle* vehicle)
{
	// Routes are now updated by the corresponding schedule actualization algorithm, so this isn't a SceneManager's responsibility now
	return true;
}

TEST_CASE("SceneManager", "[integration][functional][scene_manager]") {
    using namespace Scheduler;

    CrowFlyRoutingService routing_service;
	Engine engine;

    SceneManager* scene_manager = engine.getSceneManager();

    Scene *scene = scene_manager->createScene();

    Location depot = make_location(0, 4);
    Location shift_start = make_location(3, 5);
    Location shift_end = make_location(2, 5);

    REQUIRE(scene != nullptr);

	INFO("[SceneManager] Checking if scene is created in default state")
    {
        REQUIRE(scene->getFreeOperations().empty());
        REQUIRE(scene->getVehicles().empty());
        REQUIRE(scene->getPerformers().empty());
        REQUIRE(scene->getSchedules().empty());
        REQUIRE(scene->getOrders().empty());
    }

    Operation *free_operation = scene->createFreeOperation();

    REQUIRE(free_operation != nullptr);

    INFO("[Scene] Checking if operation is created in default state") {
        REQUIRE(strcmp(free_operation->getName(), "") == 0);
        REQUIRE(free_operation->getDuration() == Duration(0));
        REQUIRE_FALSE(free_operation->constraints().demand().isSet());
        REQUIRE(free_operation->getOrder() == nullptr);
        REQUIRE_FALSE(free_operation->constraints().timeWindows().isSet());
        REQUIRE(free_operation->getLocation() == Location());
    }

    free_operation->setName("Operation1");
    free_operation->setDuration(Units::minutes(10));
    free_operation->setLocation(make_location(10, 45));
    free_operation->constraints().demand().set(Capacity(1, 3, 4, 5));
    free_operation->constraints().timeWindows().set({make_time_window(0, 45)});

    INFO("[Operation] Checking if operation state is changed correctly") {
        REQUIRE(strcmp(free_operation->getName(), "Operation1") == 0);
        REQUIRE(free_operation->getDuration() == Units::minutes(10));
        REQUIRE(free_operation->constraints().demand().get() == Capacity(1, 3, 4, 5));
        REQUIRE(free_operation->getOrder() == nullptr);
        REQUIRE(free_operation->constraints().timeWindows().get().size() == 1);
        REQUIRE(free_operation->constraints().timeWindows().get()[0] == make_time_window(0, 45));
        REQUIRE(free_operation->getLocation() == make_location(10, 45));
    }

    const Attribute *attr1 = scene_manager->createAttribute("attr1");
    const Attribute *attr2 = scene_manager->createAttribute("attr2");
    const Attribute *skill1 = scene_manager->createAttribute("skill1");
    const Attribute *skill2 = scene_manager->createAttribute("skill2");

    Vehicle *vehicle = scene->createVehicle();

    REQUIRE(vehicle != nullptr);

    INFO("[Scene] Checking if vehicle is created in default state") {
        REQUIRE(strcmp(vehicle->getName(), "") == 0);
        REQUIRE(vehicle->getActivationCost() == Cost(0));
        REQUIRE(vehicle->getDurationUnitCost() == Cost(0));
        REQUIRE(vehicle->getDistanceUnitCost() == Cost(0));
        REQUIRE(vehicle->getAttributes().empty());
        REQUIRE_FALSE(vehicle->constraints().capacity().isSet());
        REQUIRE(vehicle->getRoutingProfile() == RoutingProfile());
        REQUIRE_FALSE(vehicle->constraints().availabilityWindows().isSet());
    }

    RoutingProfile defaultRoutingProfile;
    defaultRoutingProfile.setAverageSpeed(Speed(Distance(35)));

    vehicle->setName("Vehicle1");
    vehicle->setAttributes({attr1, attr2});
    vehicle->setActivationCost(Cost(4));
    vehicle->constraints().availabilityWindows().set({make_time_window(0, 45)});
    vehicle->constraints().capacity().set(Capacity(1, 2, 3, 4));
    vehicle->setDistanceUnitCost(Cost(4));
    vehicle->setDurationUnitCost(Cost(5));
    vehicle->setRoutingProfile(defaultRoutingProfile);

    INFO("[Vehicle] Checking if vehicle state is changed correctly") {
        REQUIRE(strcmp(vehicle->getName(), "Vehicle1") == 0);
        REQUIRE(vehicle->getActivationCost() == Cost(4));
        REQUIRE(vehicle->getDurationUnitCost() == Cost(5));
        REQUIRE(vehicle->getDistanceUnitCost() == Cost(4));
        REQUIRE(vehicle->getAttributes().size() == 2);
        REQUIRE(std::contains_key(vehicle->getAttributes(), attr1));
        REQUIRE(std::contains_key(vehicle->getAttributes(), attr2));
        REQUIRE(vehicle->constraints().capacity().get() == Capacity(1, 2, 3, 4));
        REQUIRE(vehicle->getRoutingProfile() == defaultRoutingProfile);
        REQUIRE(vehicle->constraints().availabilityWindows().get().size() == 1);
        REQUIRE(vehicle->constraints().availabilityWindows().get()[0] == make_time_window(0, 45));
    }

    Performer *performer = scene->createPerformer();

    REQUIRE(performer != nullptr);

    INFO("[Scene] Checking if performer is created in default state") {
        REQUIRE(strcmp(performer->getName(), "") == 0);
        REQUIRE(performer->getActivationCost() == Cost(0));
        REQUIRE(performer->getDurationUnitCost() == Cost(0));
        REQUIRE_FALSE(performer->constraints().availabilityWindows().isSet());
        REQUIRE(performer->getSkills().empty());
    }

    performer->setName("Performer1");
    performer->constraints().availabilityWindows().set({make_time_window(0, 45)});
    performer->setDurationUnitCost(Cost(5));
    performer->setActivationCost(Cost(6));
    performer->setSkills({skill1, skill2});

    INFO("[Performer] Checking if performer state is changed correctly") {
        REQUIRE(strcmp(performer->getName(), "Performer1") == 0);
        REQUIRE(performer->getActivationCost() == Cost(6));
        REQUIRE(performer->getDurationUnitCost() == Cost(5));
        REQUIRE(performer->constraints().availabilityWindows().get().size() == 1);
        REQUIRE(performer->constraints().availabilityWindows().get()[0] == make_time_window(0, 45));
        REQUIRE(performer->getSkills().size() == 2);
        REQUIRE(std::contains_key(performer->getSkills(), skill1));
        REQUIRE(std::contains_key(performer->getSkills(), skill2));
    }

    Schedule *schedule = scene->createSchedule(performer);

    REQUIRE(schedule != nullptr);
	REQUIRE(schedule->getScene() == scene);

    INFO("[Scene] Checking if schedule is created in default state") {
        REQUIRE(strcmp(schedule->getName(), "") == 0);
        REQUIRE(schedule->getDepotLocation() == make_location(0, 0));
        REQUIRE_FALSE(schedule->constraints().shiftStartLocation().isSet());
        REQUIRE_FALSE(schedule->constraints().shiftEndLocation().isSet());
        REQUIRE(schedule->getRuns().empty());
        REQUIRE(schedule->getPerformer() == performer);
		REQUIRE(schedule->getShift() == TimeWindow());
    }

    schedule->setName("Schedule1");
    schedule->setDepotLocation(depot);
	schedule->setShift(make_time_window(0, 3600));

    INFO("[Schedule] Checking if schedule state is changed corretly") {
        REQUIRE(strcmp(schedule->getName(), "Schedule1") == 0);
        REQUIRE(schedule->getDepotLocation() == depot);
        REQUIRE_FALSE(schedule->constraints().shiftStartLocation().isSet());
        REQUIRE_FALSE(schedule->constraints().shiftEndLocation().isSet());
        REQUIRE(schedule->getRuns().empty());
        REQUIRE(schedule->getPerformer() == performer);
		REQUIRE(schedule->getShift() == make_time_window(0, 3600));
    }

    schedule->constraints().shiftEndLocation().set(shift_end);

    INFO("[Schedule] Checking if schedule end location is set correctly") {
        REQUIRE(schedule->constraints().shiftEndLocation().get() == shift_end);
        REQUIRE(schedule->constraints().shiftEndLocation().isSet());
    }

    schedule->constraints().shiftStartLocation().set(shift_start);

    INFO("[Schedule] Checking if start location is set correctly") {
        REQUIRE(schedule->constraints().shiftStartLocation().get() == shift_start);
        REQUIRE(schedule->constraints().shiftStartLocation().isSet());
    }

    Order *order = scene->createOrder();

    REQUIRE(order != nullptr);

    INFO("[Scene] Checking if order is created in default state") {
        REQUIRE(strcmp(order->getName(), "") == 0);
        REQUIRE(order->getStartOperation() == nullptr);
        REQUIRE(order->getWorkOperations().empty());
        REQUIRE(order->getEndOperation() == nullptr);
    }

    order->setName("Order1");

    INFO("[Order] Checking if order state is changed correctly") {
        REQUIRE(strcmp(order->getName(), "Order1") == 0);
        REQUIRE(order->getStartOperation() == nullptr);
        REQUIRE(order->getWorkOperations().empty());
        REQUIRE(order->getEndOperation() == nullptr);
    }

    Operation *order1_start_operation = order->createStartOperation();
    REQUIRE(order1_start_operation != nullptr);
    REQUIRE(order->getStartOperation() == order1_start_operation);

    INFO("[Order] Checking if start operation is created in default state") {
        REQUIRE(strcmp(order1_start_operation->getName(), "") == 0);
        REQUIRE(order1_start_operation->getDuration() == Duration(0));
        REQUIRE_FALSE(order1_start_operation->constraints().demand().isSet());
        REQUIRE(order1_start_operation->getOrder() == order);
        REQUIRE_FALSE(order1_start_operation->constraints().timeWindows().isSet());
        REQUIRE(order1_start_operation->getLocation() == Location());
    }

    order1_start_operation->setName("Order1.StartOperation");
    order1_start_operation->setDuration(Units::minutes(10));
    order1_start_operation->setLocation(shift_start);
    order1_start_operation->constraints().demand().set(Capacity(1, 3, 4, 5));
    order1_start_operation->constraints().timeWindows().set({make_time_window(0, 45)});

    Operation *order1_end_operation = order->createEndOperation();
    REQUIRE(order1_end_operation != nullptr);
    REQUIRE(order->getEndOperation() == order1_end_operation);

    INFO("[Order] Checking if end operation is created in default state") {
        REQUIRE(strcmp(order1_end_operation->getName(), "") == 0);
        REQUIRE(order1_end_operation->getDuration() == Duration(0));
        REQUIRE_FALSE(order1_end_operation->constraints().demand().isSet());
        REQUIRE(order1_end_operation->getOrder() == order);
        REQUIRE_FALSE(order1_end_operation->constraints().timeWindows().isSet());
        REQUIRE(order1_end_operation->getLocation() == Location());
    }

    order1_end_operation->setName("Order1.EndOperation");
    order1_end_operation->setDuration(Units::minutes(10));
    order1_end_operation->setLocation(depot);
    order1_end_operation->constraints().demand().set(Capacity(1, 3, 4, 5));
    order1_end_operation->constraints().timeWindows().set({make_time_window(0, 45)});

    Operation *order1_work_operation1 = order->createWorkOperation();
    REQUIRE(order1_work_operation1 != nullptr);
    REQUIRE(order->getWorkOperations().size() == 1);
    REQUIRE(order->getWorkOperations()[0] == order1_work_operation1);

    INFO("[Order] Checking if work operation 1 is created in default state") {
        REQUIRE(strcmp(order1_work_operation1->getName(), "") == 0);
        REQUIRE(order1_work_operation1->getDuration() == Duration(0));
        REQUIRE_FALSE(order1_work_operation1->constraints().demand().isSet());
        REQUIRE(order1_work_operation1->getOrder() == order);
        REQUIRE_FALSE(order1_work_operation1->constraints().timeWindows().isSet());
        REQUIRE(order1_work_operation1->getLocation() == Location());
    }

    order1_work_operation1->setName("Order1.WorkOperation1");
    order1_work_operation1->setDuration(Units::minutes(10));
    order1_work_operation1->setLocation(make_location(23, 45));
    order1_work_operation1->constraints().demand().set(Capacity(1, 3, 4, 5));
    order1_work_operation1->constraints().timeWindows().set({make_time_window(0, 45)});

    Operation *order1_work_operation2 = order->createWorkOperation();
    REQUIRE(order1_work_operation2 != nullptr);
    REQUIRE(order->getWorkOperations().size() == 2);
    REQUIRE(order->getWorkOperations()[0] == order1_work_operation1);
    REQUIRE(order->getWorkOperations()[1] == order1_work_operation2);

    INFO("[Order] Checking if work operation 2 is created in default state") {
        REQUIRE(strcmp(order1_work_operation2->getName(), "") == 0);
        REQUIRE(order1_work_operation2->getDuration() == Duration(0));
        REQUIRE_FALSE(order1_work_operation2->constraints().demand().isSet());
        REQUIRE(order1_work_operation2->getOrder() == order);
        REQUIRE_FALSE(order1_work_operation2->constraints().timeWindows().isSet());
        REQUIRE(order1_work_operation2->getLocation() == Location());
    }

    order1_work_operation2->setName("Order1.WorkOperation1");
    order1_work_operation2->setDuration(Units::minutes(10));
    order1_work_operation2->setLocation(make_location(25, 35));
    order1_work_operation2->constraints().demand().set(Capacity(1, 3, 4, 5));
    order1_work_operation2->constraints().timeWindows().set({make_time_window(0, 45)});

    Run* run1 = *schedule->createRun(schedule->getRuns().end(), shift_start, depot);
    REQUIRE(run1 != nullptr);

    INFO("[Schedule] Checking if run 1 is created in default state")
    {
        REQUIRE(run1->getStartStop() != nullptr);
        REQUIRE(run1->getStartStop()->getOperations().empty());
        REQUIRE(run1->getStartStop()->getLocation() == shift_start);
        REQUIRE(run1->getEndStop()->getOperations().empty());
        REQUIRE(run1->getEndStop()->getLocation() == depot);
        REQUIRE(run1->getWorkStops().empty());
        REQUIRE(run1->getSchedule() == schedule);
        REQUIRE(run1->getVehicle() == nullptr);
    }

    Run* run0 = *schedule->createRun(schedule->getRuns().begin(), shift_start, shift_start);
    REQUIRE(run0 != nullptr);

    INFO("[Schedule] Checking if run 0 is created in default state")
    {
        REQUIRE(run0->getStartStop() != nullptr);
        REQUIRE(run0->getStartStop()->getOperations().empty());
        REQUIRE(run0->getStartStop()->getLocation() == shift_start);
        REQUIRE(run0->getEndStop()->getOperations().empty());
        REQUIRE(run0->getEndStop()->getLocation() == shift_start);
        REQUIRE(run0->getWorkStops().empty());
        REQUIRE(run0->getSchedule() == schedule);
        REQUIRE(run0->getVehicle() == nullptr);
    }

    Run* run2 = *schedule->createRun(schedule->getRuns().end(), depot, shift_end);
    REQUIRE(run2 != nullptr);

    INFO("[Schedule] Checking if run 2 is created in default state")
    {
        REQUIRE(run2->getStartStop() != nullptr);
        REQUIRE(run2->getStartStop()->getOperations().empty());
        REQUIRE(run2->getStartStop()->getLocation() == depot);
        REQUIRE(run2->getEndStop()->getOperations().empty());
        REQUIRE(run2->getEndStop()->getLocation() == shift_end);
        REQUIRE(run2->getWorkStops().empty());
        REQUIRE(run2->getSchedule() == schedule);
        REQUIRE(run2->getVehicle() == nullptr);
    }

    INFO("[Schedule] Checking if runs are ordered correctly")
    {
        REQUIRE(schedule->getRuns().size() == 3);
        REQUIRE(schedule->getRuns()[0] == run0);
        REQUIRE(schedule->getRuns()[1] == run1);
        REQUIRE(schedule->getRuns()[2] == run2);
    }

    WorkStop* free_operation_stop = *run1->createWorkStop(run1->getWorkStops().begin(), free_operation);
    REQUIRE(free_operation_stop != nullptr);

    INFO("[Run] Checking if stop is created in default state")
    {
        REQUIRE(free_operation_stop->getLocation() == free_operation->getLocation());
        REQUIRE(free_operation_stop->getAllocationTime() == TimeWindow());
        REQUIRE(free_operation_stop->getDuration() == Duration());
        REQUIRE(free_operation_stop->getOperation() == free_operation);
        REQUIRE(free_operation_stop->getRun() == run1);
    }

	WorkStop* work_stop1 = *run1->createWorkStop(std::next(run1->getWorkStops().begin()), order1_work_operation2);
	REQUIRE(work_stop1 != nullptr);

	INFO("[Run] Checking if stop is created in default state")
	{
		REQUIRE(work_stop1->getLocation() == order1_work_operation2->getLocation());
		REQUIRE(work_stop1->getAllocationTime() == TimeWindow());
		REQUIRE(work_stop1->getDuration() == Duration());
		REQUIRE(work_stop1->getOperation() == order1_work_operation2);
		REQUIRE(work_stop1->getRun() == run1);
	}

	WorkStop* work_stop0 = *run1->createWorkStop(run1->getWorkStops().begin(), order1_work_operation1);
	REQUIRE(work_stop0 != nullptr);

	INFO("[Run] Checking if stop is created in default state")
	{
		REQUIRE(work_stop0->getLocation() == order1_work_operation1->getLocation());
		REQUIRE(work_stop0->getAllocationTime() == TimeWindow());
		REQUIRE(work_stop0->getDuration() == Duration());
		REQUIRE(work_stop0->getOperation() == order1_work_operation1);
		REQUIRE(work_stop0->getRun() == run1);
	}

	INFO("[Run] Checking if stops are ordered correctly")
	{
		REQUIRE(run1->getWorkStops().size() == 3);
		REQUIRE(*run1->getWorkStops().begin() == work_stop0);
		REQUIRE(*std::next(run1->getWorkStops().begin(), 1) == free_operation_stop);
		REQUIRE(*std::next(run1->getWorkStops().begin(), 2) == work_stop1);
	}

	INFO("[Run] Checking if only work stops are allocated")
	{
		REQUIRE(run1->getStartStop()->getOperations().empty());
		REQUIRE(run1->getEndStop()->getOperations().empty());
	}

	RunBoundaryStop* start_stop = run1->allocateStartOperation(order1_start_operation);
	REQUIRE(start_stop != nullptr);

	INFO("[Run] Checking if start operation allocated")
	{
		REQUIRE(start_stop == run1->getStartStop());
		REQUIRE(start_stop->getOperations().size() == 1);
		REQUIRE(std::contains_key(start_stop->getOperations(), order1_start_operation));
	}

	RunBoundaryStop* end_stop = run1->allocateEndOperation(order1_end_operation);
	REQUIRE(end_stop != nullptr);

	INFO("[Run] Checking if end operation allocated")
	{
		REQUIRE(end_stop == run1->getEndStop());
		REQUIRE(end_stop->getOperations().size() == 1);
		REQUIRE(std::contains_key(end_stop->getOperations(), order1_end_operation));
	}

	run1->setVehicle(vehicle);

	INFO("[Run] Checking if routes are set correctly")
	{
		REQUIRE(checkRoute(run1->getStartStop(), *run1->getWorkStops().begin(), &routing_service, vehicle));
		REQUIRE(checkRoute(*run1->getWorkStops().begin(), *std::next(run1->getWorkStops().begin()), &routing_service, vehicle));
		REQUIRE(checkRoute(*std::next(run1->getWorkStops().begin(), 1), *std::next(run1->getWorkStops().begin(), 2), &routing_service, vehicle));
		REQUIRE(checkRoute(*std::next(run1->getWorkStops().begin(), 2), run1->getEndStop(), &routing_service, vehicle));
	}

	run1->unallocateStartOperation(order1_start_operation);
	run1->unallocateEndOperation(order1_end_operation);
	run1->destroyWorkStop( std::find_if(run1->getWorkStops().begin(), run1->getWorkStops().end(), [&](WorkStop* stop){return stop->getOperation() == free_operation; } ));
	run1->destroyWorkStop(run1->getWorkStops().begin());

	INFO("[Run] Checking if operations are unallocated correctly")
	{
		REQUIRE(run1->getStartStop()->getOperations().empty());
		REQUIRE(run1->getEndStop()->getOperations().empty());
		REQUIRE(run1->getWorkStops().size() == 1);
		REQUIRE(*run1->getWorkStops().begin() == work_stop1);
	}

	INFO("Checking schedule temporary copy")
	{
		Schedule* schedule = scene->getSchedules()[0];

		TemporarySchedule temporary_schedule = scene->createTemporaryScheduleCopy(schedule);

		REQUIRE(strcmp(temporary_schedule->getName(), schedule->getName()) == 0);
		temporary_schedule->setName("AnotherName");
		REQUIRE(strcmp(temporary_schedule->getName(), schedule->getName()) != 0);
		REQUIRE(temporary_schedule->getPerformer() == schedule->getPerformer());
		REQUIRE(temporary_schedule->getDepotLocation() == schedule->getDepotLocation());
		REQUIRE(temporary_schedule->getShift() == schedule->getShift());
		REQUIRE(temporary_schedule->constraints().shiftStartLocation().get() == schedule->constraints().shiftStartLocation().get());
		REQUIRE(temporary_schedule->constraints().shiftEndLocation().get() == schedule->constraints().shiftEndLocation().get());
		REQUIRE(temporary_schedule->getScene() == scene);

		REQUIRE(temporary_schedule->getRuns().size() == schedule->getRuns().size());
		
		for (size_t i = 0; i < temporary_schedule->getRuns().size(); ++i)
		{
			Run *orig = schedule->getRuns()[i];
			Run *temp = temporary_schedule->getRuns()[i];

			REQUIRE(orig->getVehicle() == temp->getVehicle());

			REQUIRE(orig->getWorkStops().size() == temp->getWorkStops().size());

			REQUIRE(orig->getStartStop()->getOperations() == temp->getStartStop()->getOperations());
			REQUIRE(orig->getStartStop()->getLocation() == temp->getStartStop()->getLocation());
			REQUIRE(orig->getStartStop()->getDuration() == temp->getStartStop()->getDuration());
			REQUIRE(orig->getStartStop()->getAllocationTime() == temp->getStartStop()->getAllocationTime());

			REQUIRE(orig->getEndStop()->getOperations() == temp->getEndStop()->getOperations());
			REQUIRE(orig->getEndStop()->getLocation() == temp->getEndStop()->getLocation());
			REQUIRE(orig->getEndStop()->getDuration() == temp->getEndStop()->getDuration());
			REQUIRE(orig->getEndStop()->getAllocationTime() == temp->getEndStop()->getAllocationTime());

			for (size_t j = 0; j < orig->getWorkStops().size(); ++j)
			{
				WorkStop *sorig = *std::next(orig->getWorkStops().begin(), j);
				WorkStop *stemp = *std::next(temp->getWorkStops().begin(), j);

				REQUIRE(sorig->getAllocationTime() == stemp->getAllocationTime());
				REQUIRE(sorig->getDuration() == stemp->getDuration());
				REQUIRE(sorig->getLocation() == stemp->getLocation());
				REQUIRE(sorig->getNextRoute() == stemp->getNextRoute());
				REQUIRE(sorig->getOperation() == stemp->getOperation());
			}
		}

		Schedule* temp_schedule = temporary_schedule.get();
		TemporarySchedule alternative_pointer;
		alternative_pointer = std::move(temporary_schedule);
		REQUIRE(alternative_pointer.get() == temp_schedule);
		
	}

    scene_manager->destroyScene(scene);
}


TEST_CASE("Run operations")
{
	using namespace Scheduler;
	
	Engine engine;
	SceneManager *sm = engine.getSceneManager();
	Scene* scene = sm->createScene();
	
	Performer* performer = scene->createPerformer();
	Vehicle* vehicle = scene->createVehicle();
	
	Operation* operation1 = scene->createFreeOperation();
	operation1->setName("1");
	Operation* operation2 = scene->createFreeOperation();
	operation2->setName("2");
	Operation* operation3 = scene->createFreeOperation();
	operation3->setName("3");
	Operation* operation4 = scene->createFreeOperation();
	operation4->setName("4");
	
	Schedule* schedule = scene->createSchedule(performer);
	Run* r = *schedule->createRun(schedule->getRuns().end(), Location(), Location());
	
	Run::WorkStopsList::iterator iter1 = r->createWorkStop(r->getWorkStops().end(), operation1);
	Run::WorkStopsList::iterator iter2 = r->createWorkStop(r->getWorkStops().end(), operation2);
	Run::WorkStopsList::iterator iter3 = r->createWorkStop(r->getWorkStops().end(), operation3);
	Run::WorkStopsList::iterator iter4 = r->createWorkStop(r->getWorkStops().end(), operation4);
	Run::WorkStopsList::iterator end = r->getWorkStops().end();
	
	REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation1);
	REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
	REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
	REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
	
	SECTION("Swap")
	{
		r->swapWorkStops(iter1, iter4);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->swapWorkStops(iter2, iter3);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->swapWorkStops(iter4, iter2);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->swapWorkStops(iter3, iter1);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		
		r->swapWorkStops(iter1, iter1);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
	}
	
	SECTION("Reverse")
	{
		r->reverseWorkStops(iter1, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
	
		r->reverseWorkStops(iter4, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->reverseWorkStops(iter1, iter3);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->reverseWorkStops(iter3, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		
		r->reverseWorkStops(iter1, iter4);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
		
		r->reverseWorkStops(iter1, iter1);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation3);
	}
	
	SECTION("Self splice")
	{
		r->spliceOwnWorkStops(r->getWorkStops().begin(), iter3, iter4);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->spliceOwnWorkStops(r->getWorkStops().end(), iter1, iter2);

		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->spliceOwnWorkStops(r->getWorkStops().begin(), iter4, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation2);
		
		r->spliceOwnWorkStops(r->getWorkStops().end(), iter4, iter3);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation4);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation1);
		
		r->spliceOwnWorkStops(iter2, iter1, end);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
		
		r->spliceOwnWorkStops(iter4, iter3, iter2);
		
		REQUIRE((*std::next(r->getWorkStops().begin(), 0))->getOperation() == operation2);
		REQUIRE((*std::next(r->getWorkStops().begin(), 1))->getOperation() == operation3);
		REQUIRE((*std::next(r->getWorkStops().begin(), 2))->getOperation() == operation1);
		REQUIRE((*std::next(r->getWorkStops().begin(), 3))->getOperation() == operation4);
	}
	
}
