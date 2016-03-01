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

bool checkRoute(Scheduler::Stop* from, Scheduler::Stop* to, Scheduler::RoutingService* routing_service, Scheduler::Vehicle* vehicle)
{
	return from->getNextRoute() == routing_service->calculateRoute(from->getLocation(), to->getLocation(), vehicle->getRoutingProfile());
}

TEST_CASE("SceneManager", "[unit][integration][scene_manager]") {
    using namespace Scheduler;

    CrowFlyRoutingService routing_service;

    SceneManager scene_manager;
    scene_manager.setRoutingService(&routing_service);

    Scene *scene = scene_manager.createScene();

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
        REQUIRE(free_operation->getLoad() == Capacity(0, 0, 0, 0));
        REQUIRE(free_operation->getOrder() == nullptr);
        REQUIRE(free_operation->getTimeWindows().empty());
        REQUIRE(free_operation->getLocation() == Location());
    }

    free_operation->setName("Operation1");
    free_operation->setDuration(Units::minutes(10));
    free_operation->setLocation(make_location(10, 45));
    free_operation->setLoad(Capacity(1, 3, 4, 5));
    free_operation->setTimeWindows({make_time_window(0, 45)});

    INFO("[Operation] Checking if operation state is changed correctly") {
        REQUIRE(strcmp(free_operation->getName(), "Operation1") == 0);
        REQUIRE(free_operation->getDuration() == Units::minutes(10));
        REQUIRE(free_operation->getLoad() == Capacity(1, 3, 4, 5));
        REQUIRE(free_operation->getOrder() == nullptr);
        REQUIRE(free_operation->getTimeWindows().size() == 1);
        REQUIRE(free_operation->getTimeWindows()[0] == make_time_window(0, 45));
        REQUIRE(free_operation->getLocation() == make_location(10, 45));
    }

    const Attribute *attr1 = scene_manager.createAttribute("attr1");
    const Attribute *attr2 = scene_manager.createAttribute("attr2");
    const Attribute *skill1 = scene_manager.createAttribute("skill1");
    const Attribute *skill2 = scene_manager.createAttribute("skill2");

    Vehicle *vehicle = scene->createVehicle();

    REQUIRE(vehicle != nullptr);

    INFO("[Scene] Checking if vehicle is created in default state") {
        REQUIRE(strcmp(vehicle->getName(), "") == 0);
        REQUIRE(vehicle->getActivationCost() == Cost(0));
        REQUIRE(vehicle->getDurationUnitCost() == Cost(0));
        REQUIRE(vehicle->getDistanceUnitCost() == Cost(0));
        REQUIRE(vehicle->getAttributes().empty());
        REQUIRE(vehicle->getCapacity() == Capacity(0, 0, 0, 0));
        REQUIRE(vehicle->getRoutingProfile() == RoutingProfile());
        REQUIRE(vehicle->getAvailabilityWindows().empty());
    }

    RoutingProfile defaultRoutingProfile;
    defaultRoutingProfile.setAverageSpeed(Speed(Distance(35)));

    vehicle->setName("Vehicle1");
    vehicle->setAttributes({attr1, attr2});
    vehicle->setActivationCost(Cost(4));
    vehicle->setAvailabilityWindows({make_time_window(0, 45)});
    vehicle->setCapacity(Capacity(1, 2, 3, 4));
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
        REQUIRE(vehicle->getCapacity() == Capacity(1, 2, 3, 4));
        REQUIRE(vehicle->getRoutingProfile() == defaultRoutingProfile);
        REQUIRE(vehicle->getAvailabilityWindows().size() == 1);
        REQUIRE(vehicle->getAvailabilityWindows()[0] == make_time_window(0, 45));
    }

    Performer *performer = scene->createPerformer();

    REQUIRE(performer != nullptr);

    INFO("[Scene] Checking if performer is created in default state") {
        REQUIRE(strcmp(performer->getName(), "") == 0);
        REQUIRE(performer->getActivationCost() == Cost(0));
        REQUIRE(performer->getDurationUnitCost() == Cost(0));
        REQUIRE(performer->getAvailabilityWindows().empty());
        REQUIRE(performer->getSkills().empty());
    }

    performer->setName("Performer1");
    performer->setAvailabilityWindows({make_time_window(0, 45)});
    performer->setDurationUnitCost(Cost(5));
    performer->setActivationCost(Cost(6));
    performer->setSkills({skill1, skill2});

    INFO("[Performer] Checking if performer state is changed correctly") {
        REQUIRE(strcmp(performer->getName(), "Performer1") == 0);
        REQUIRE(performer->getActivationCost() == Cost(6));
        REQUIRE(performer->getDurationUnitCost() == Cost(5));
        REQUIRE(performer->getAvailabilityWindows().size() == 1);
        REQUIRE(performer->getAvailabilityWindows()[0] == make_time_window(0, 45));
        REQUIRE(performer->getSkills().size() == 2);
        REQUIRE(std::contains_key(performer->getSkills(), skill1));
        REQUIRE(std::contains_key(performer->getSkills(), skill2));
    }

    Schedule *schedule = scene->createSchedule(performer);

    schedule->getScheduleActualizer()->createAlgorithm<StopDurationActualizationAlgorithm>();
    schedule->getScheduleActualizer()->createAlgorithm<StopArrivalTimeActualizationAlgorithm>();

    REQUIRE(schedule != nullptr);

    INFO("[Scene] Checking if schedule is created in default state") {
        REQUIRE(strcmp(schedule->getName(), "") == 0);
        REQUIRE(schedule->getDepotLocation() == make_location(0, 0));
        REQUIRE(schedule->getShiftStartLocation() == schedule->getDepotLocation());
        REQUIRE(schedule->getShiftEndLocation() == schedule->getDepotLocation());
        REQUIRE_FALSE(schedule->hasSpecificStartLocation());
        REQUIRE_FALSE(schedule->hasSpecificEndLocation());
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
        REQUIRE(schedule->getShiftStartLocation() == schedule->getDepotLocation());
        REQUIRE(schedule->getShiftEndLocation() == schedule->getDepotLocation());
        REQUIRE_FALSE(schedule->hasSpecificStartLocation());
        REQUIRE_FALSE(schedule->hasSpecificEndLocation());
        REQUIRE(schedule->getRuns().empty());
        REQUIRE(schedule->getPerformer() == performer);
		REQUIRE(schedule->getShift() == make_time_window(0, 3600));
    }

    schedule->setShiftEndLocation(shift_end);

    INFO("[Schedule] Checking if schedule end location is set correctly") {
        REQUIRE(schedule->getShiftEndLocation() == shift_end);
        REQUIRE(schedule->hasSpecificEndLocation());
        REQUIRE(schedule->hasSpecificEndLocation());
    }

    schedule->setShiftStartLocation(shift_start);

    INFO("[Schedule] Checking if start location is set correctly") {
        REQUIRE(schedule->getShiftStartLocation() == shift_start);
        REQUIRE(schedule->hasSpecificStartLocation());
    }

    Order *order = scene->createOrder();

    REQUIRE(order != nullptr);

    INFO("[Scene] Checking if order is created in default state") {
        REQUIRE(strcmp(order->getName(), "") == 0);
        REQUIRE(order->getPerformerSkillsRequirements().empty());
        REQUIRE(order->getVehicleRequirements().empty());
        REQUIRE(order->getStartOperation() == nullptr);
        REQUIRE(order->getWorkOperations().empty());
        REQUIRE(order->getEndOperation() == nullptr);
    }

    order->setName("Order1");
    order->setPerformerSkillsRequirements({skill1, skill2});
    order->setVehicleRequirements({attr1, attr2});

    INFO("[Order] Checking if order state is changed correctly") {
        REQUIRE(strcmp(order->getName(), "Order1") == 0);
        REQUIRE(order->getPerformerSkillsRequirements().size() == 2);
        REQUIRE(std::contains_key(order->getPerformerSkillsRequirements(), skill1));
        REQUIRE(std::contains_key(order->getPerformerSkillsRequirements(), skill2));
        REQUIRE(order->getVehicleRequirements().size() == 2);
        REQUIRE(std::contains_key(order->getVehicleRequirements(), attr1));
        REQUIRE(std::contains_key(order->getVehicleRequirements(), attr2));
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
        REQUIRE(order1_start_operation->getLoad() == Capacity(0, 0, 0, 0));
        REQUIRE(order1_start_operation->getOrder() == order);
        REQUIRE(order1_start_operation->getTimeWindows().empty());
        REQUIRE(order1_start_operation->getLocation() == Location());
    }

    order1_start_operation->setName("Order1.StartOperation");
    order1_start_operation->setDuration(Units::minutes(10));
    order1_start_operation->setLocation(shift_start);
    order1_start_operation->setLoad(Capacity(1, 3, 4, 5));
    order1_start_operation->setTimeWindows({make_time_window(0, 45)});

    Operation *order1_end_operation = order->createEndOperation();
    REQUIRE(order1_end_operation != nullptr);
    REQUIRE(order->getEndOperation() == order1_end_operation);

    INFO("[Order] Checking if end operation is created in default state") {
        REQUIRE(strcmp(order1_end_operation->getName(), "") == 0);
        REQUIRE(order1_end_operation->getDuration() == Duration(0));
        REQUIRE(order1_end_operation->getLoad() == Capacity(0, 0, 0, 0));
        REQUIRE(order1_end_operation->getOrder() == order);
        REQUIRE(order1_end_operation->getTimeWindows().empty());
        REQUIRE(order1_end_operation->getLocation() == Location());
    }

    order1_end_operation->setName("Order1.EndOperation");
    order1_end_operation->setDuration(Units::minutes(10));
    order1_end_operation->setLocation(depot);
    order1_end_operation->setLoad(Capacity(1, 3, 4, 5));
    order1_end_operation->setTimeWindows({make_time_window(0, 45)});

    Operation *order1_work_operation1 = order->createWorkOperation();
    REQUIRE(order1_work_operation1 != nullptr);
    REQUIRE(order->getWorkOperations().size() == 1);
    REQUIRE(order->getWorkOperations()[0] == order1_work_operation1);

    INFO("[Order] Checking if work operation 1 is created in default state") {
        REQUIRE(strcmp(order1_work_operation1->getName(), "") == 0);
        REQUIRE(order1_work_operation1->getDuration() == Duration(0));
        REQUIRE(order1_work_operation1->getLoad() == Capacity(0, 0, 0, 0));
        REQUIRE(order1_work_operation1->getOrder() == order);
        REQUIRE(order1_work_operation1->getTimeWindows().empty());
        REQUIRE(order1_work_operation1->getLocation() == Location());
    }

    order1_work_operation1->setName("Order1.WorkOperation1");
    order1_work_operation1->setDuration(Units::minutes(10));
    order1_work_operation1->setLocation(make_location(23, 45));
    order1_work_operation1->setLoad(Capacity(1, 3, 4, 5));
    order1_work_operation1->setTimeWindows({make_time_window(0, 45)});

    Operation *order1_work_operation2 = order->createWorkOperation();
    REQUIRE(order1_work_operation2 != nullptr);
    REQUIRE(order->getWorkOperations().size() == 2);
    REQUIRE(order->getWorkOperations()[0] == order1_work_operation1);
    REQUIRE(order->getWorkOperations()[1] == order1_work_operation2);

    INFO("[Order] Checking if work operation 2 is created in default state") {
        REQUIRE(strcmp(order1_work_operation2->getName(), "") == 0);
        REQUIRE(order1_work_operation2->getDuration() == Duration(0));
        REQUIRE(order1_work_operation2->getLoad() == Capacity(0, 0, 0, 0));
        REQUIRE(order1_work_operation2->getOrder() == order);
        REQUIRE(order1_work_operation2->getTimeWindows().empty());
        REQUIRE(order1_work_operation2->getLocation() == Location());
    }

    order1_work_operation2->setName("Order1.WorkOperation1");
    order1_work_operation2->setDuration(Units::minutes(10));
    order1_work_operation2->setLocation(make_location(25, 35));
    order1_work_operation2->setLoad(Capacity(1, 3, 4, 5));
    order1_work_operation2->setTimeWindows({make_time_window(0, 45)});

    Run* run1 = schedule->createRun(shift_start, depot);
    REQUIRE(run1 != nullptr);

    INFO("[Schedule] Checking if run 1 is created in default state")
    {
        REQUIRE(run1->getStartLocation() == shift_start);
        REQUIRE(run1->getEndLocation() == depot);
        REQUIRE(run1->getStartStop() != nullptr);
        REQUIRE(run1->getStartStop()->getOperations().empty());
        REQUIRE(run1->getStartStop()->getLocation() == shift_start);
        REQUIRE(run1->getEndStop()->getOperations().empty());
        REQUIRE(run1->getEndStop()->getLocation() == depot);
        REQUIRE(run1->getWorkStops().empty());
        REQUIRE(run1->getSchedule() == schedule);
        REQUIRE(run1->getVehicle() == nullptr);
    }

    Run* run0 = schedule->createRun(shift_start, shift_start, 0);
    REQUIRE(run0 != nullptr);

    INFO("[Schedule] Checking if run 0 is created in default state")
    {
        REQUIRE(run0->getStartLocation() == shift_start);
        REQUIRE(run0->getEndLocation() == shift_start);
        REQUIRE(run0->getStartStop() != nullptr);
        REQUIRE(run0->getStartStop()->getOperations().empty());
        REQUIRE(run0->getStartStop()->getLocation() == shift_start);
        REQUIRE(run0->getEndStop()->getOperations().empty());
        REQUIRE(run0->getEndStop()->getLocation() == shift_start);
        REQUIRE(run0->getWorkStops().empty());
        REQUIRE(run0->getSchedule() == schedule);
        REQUIRE(run0->getVehicle() == nullptr);
    }

    Run* run2 = schedule->createRun(depot, shift_end, 2);
    REQUIRE(run2 != nullptr);

    INFO("[Schedule] Checking if run 2 is created in default state")
    {
        REQUIRE(run2->getStartLocation() == depot);
        REQUIRE(run2->getEndLocation() == shift_end);
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

    Stop* free_operation_stop = run1->allocateWorkOperation(free_operation, 0);
    REQUIRE(free_operation_stop != nullptr);

    INFO("[Run] Checking if stop is created in default state")
    {
        REQUIRE(free_operation_stop->getLocation() == free_operation->getLocation());
        //REQUIRE(free_operation_stop->getAllocationTime() == TimeWindow()); is no more actual due to the schedule actualization algorithm
        REQUIRE(free_operation_stop->getDuration() == Duration());
        REQUIRE(free_operation_stop->getOperations().size() == 1);
		REQUIRE(std::contains_key(free_operation_stop->getOperations(), free_operation));
        REQUIRE(free_operation_stop->getRun() == run1);
    }

	Stop* work_stop1 = run1->allocateWorkOperation(order1_work_operation2, 1);
	REQUIRE(work_stop1 != nullptr);

	INFO("[Run] Checking if stop is created in default state")
	{
		REQUIRE(work_stop1->getLocation() == order1_work_operation2->getLocation());
		//REQUIRE(work_stop1->getAllocationTime() == TimeWindow()); is no more actual due to the schedule actualization algorithm
		REQUIRE(work_stop1->getDuration() == Duration());
		REQUIRE(work_stop1->getOperations().size() == 1);
		REQUIRE(std::contains_key(work_stop1->getOperations(), order1_work_operation2));
		REQUIRE(work_stop1->getRun() == run1);
	}

	Stop* work_stop0 = run1->allocateWorkOperation(order1_work_operation1, 0);
	REQUIRE(work_stop0 != nullptr);

	INFO("[Run] Checking if stop is created in default state")
	{
		REQUIRE(work_stop0->getLocation() == order1_work_operation1->getLocation());
		//REQUIRE(work_stop0->getAllocationTime() == TimeWindow()); is no more actual due to the schedule actualization algorithm
		REQUIRE(work_stop0->getDuration() == Duration());
		REQUIRE(work_stop0->getOperations().size() == 1);
		REQUIRE(std::contains_key(work_stop0->getOperations(), order1_work_operation1));
		REQUIRE(work_stop0->getRun() == run1);
	}

	INFO("[Run] Checking if stops are ordered correctly")
	{
		REQUIRE(run1->getWorkStops().size() == 3);
		REQUIRE(run1->getWorkStops()[0] == work_stop0);
		REQUIRE(run1->getWorkStops()[1] == free_operation_stop);
		REQUIRE(run1->getWorkStops()[2] == work_stop1);
	}

	INFO("[Run] Checking if only work stops are allocated")
	{
		REQUIRE(run1->getStartStop()->getOperations().empty());
		REQUIRE(run1->getEndStop()->getOperations().empty());
	}

	Stop* start_stop = run1->allocateStartOperation(order1_start_operation);
	REQUIRE(start_stop != nullptr);

	INFO("[Run] Checking if start operation allocated")
	{
		REQUIRE(start_stop == run1->getStartStop());
		REQUIRE(start_stop->getOperations().size() == 1);
		REQUIRE(std::contains_key(start_stop->getOperations(), order1_start_operation));
	}

	Stop* end_stop = run1->allocateEndOperation(order1_end_operation);
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
		REQUIRE(checkRoute(run1->getStartStop(), run1->getWorkStops()[0], &routing_service, vehicle));
		REQUIRE(checkRoute(run1->getWorkStops()[0], run1->getWorkStops()[1], &routing_service, vehicle));
		REQUIRE(checkRoute(run1->getWorkStops()[1], run1->getWorkStops()[2], &routing_service, vehicle));
		REQUIRE(checkRoute(run1->getWorkStops()[2], run1->getEndStop(), &routing_service, vehicle));
	}

	run1->unallocateStartOperation(order1_start_operation);
	run1->unallocateEndOperation(order1_end_operation);
	run1->unallocateWorkOperation(free_operation);
	run1->unallocateWorkOperationAt(0);

	INFO("[Run] Checking if operations are unallocated correctly")
	{
		REQUIRE(run1->getStartStop()->getOperations().empty());
		REQUIRE(run1->getEndStop()->getOperations().empty());
		REQUIRE(run1->getWorkStops().size() == 1);
		REQUIRE(run1->getWorkStops()[0] == work_stop1);
	}

    scene_manager.destroyScene(scene);
}
