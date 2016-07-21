#include <assert.h>
#include "Scene.h"
#include "Operation.h"
#include "Schedule.h"
#include "Vehicle.h"
#include "Performer.h"
#include "ScheduleActualizationModel.h"

namespace Scheduler {

    Scene::Scene(size_t id) :
            id(id),
            operations_factory(nullptr),
            orders_factory(nullptr),
            performers_factory(nullptr),
            vehicles_factory(nullptr),
            schedules_factory(nullptr),
            run_vehicle_binders_factory(nullptr),
            run_vehicle_binder(nullptr)
    {
    }

    size_t Scene::getId() const {
        return id;
    }

    const ImmutableVector<Operation*>& Scene::getFreeOperations() const {
        return free_operations;
    }

	const ImmutableVector<Order*>& Scene::getOrders() const {
        return orders;
    }

	const ImmutableVector<Schedule*>& Scene::getSchedules() const {
        return schedules;
    }

	ImmutableVector<Operation*>& Scene::getFreeOperations(){
		return free_operations;
	}

	ImmutableVector<Order*>& Scene::getOrders(){
		return orders;
	}

	ImmutableVector<Schedule*>& Scene::getSchedules(){
		return schedules;
	}

	const SceneConstraints & Scene::constraints() const
	{
		return scene_constraints;
	}

	SceneConstraints & Scene::constraints()
	{
		return scene_constraints;
	}

	void Scene::setOperationsfactory(SceneObjectsFactory<Operation> *factory) {
        this->operations_factory = factory;
    }

    void Scene::setOrdersFactory(SceneObjectsFactory<Order> *factory) {
        this->orders_factory = factory;
    }

    void Scene::setPerformersFactory(SceneObjectsFactory<Performer> *factory) {
        this->performers_factory = factory;
    }

    void Scene::setVehiclesFactory(SceneObjectsFactory<Vehicle> *factory) {
        this->vehicles_factory = factory;
    }

    void Scene::setSchedulesFactory(SceneObjectsFactory<Schedule> *factory) {
        this->schedules_factory = factory;
    }

    Operation *Scene::createFreeOperation() {
        assert(operations_factory);
        if(!operations_factory) return nullptr;

        Operation* operation = operations_factory->createObject();
        free_operations.push_back(operation);

        return operation;
    }

    Order *Scene::createOrder() {
        assert(orders_factory);
        if(!orders_factory) return nullptr;

        Order* order = orders_factory->createObject();
		order->setOperationsFactory(operations_factory);
        orders.push_back(order);

        return order;
    }

    Schedule *Scene::createSchedule(const Performer* performer) {
        assert(schedules_factory);
        if(!schedules_factory) return nullptr;

        Schedule* schedule = schedules_factory->createObject(performer);
		schedule->setScene(this);
		schedule->setRunsFactory(runs_factory);
		schedule->setStopsFactory(stops_factory);
        schedule->setRunVehicleBinder(run_vehicle_binder);

        schedules.push_back(schedule);

        return schedule;
    }

	TemporarySchedule Scene::createTemporaryScheduleCopy(Schedule * schedule) const
	{
		Schedule* new_schedule = schedules_factory->createObject(schedule);

		return std::move(TemporarySchedule(new_schedule, TemporaryScheduleDeleter(schedules_factory)));
	}

    Scene::~Scene() {
        assert(run_vehicle_binders_factory);
        if(run_vehicle_binder) run_vehicle_binders_factory->destroyObject(run_vehicle_binder);

        assert(orders_factory);
        for(Order* order : orders)
        {
            orders_factory->destroyObject(order);
        }

        assert(operations_factory);
        for(Operation* operation : free_operations)
        {
            operations_factory->destroyObject(operation);
        }

        assert(schedules_factory);
        for(Schedule* schedule : schedules)
        {
            schedules_factory->destroyObject(schedule);
        }

        assert(vehicles_factory);
        for(Vehicle* vehicle : vehicles)
        {
            vehicles_factory->destroyObject(vehicle);
        }

        assert(performers_factory);
        for(Performer* performer : performers)
        {
            performers_factory->destroyObject(performer);
        }
    }

	const ImmutableVector<Performer*> &Scene::getPerformers() const {
		return performers;
	}

	const ImmutableVector<Vehicle*> &Scene::getVehicles() const {
		return vehicles;
	}

	ImmutableVector<Performer*> &Scene::getPerformers() {
		return performers;
	}

	ImmutableVector<Vehicle*> &Scene::getVehicles() {
		return vehicles;
	}

	Performer *Scene::createPerformer() {
		assert(performers_factory);
		if(!performers_factory) return nullptr;

		Performer* performer = performers_factory->createObject();

		performers.push_back(performer);

		return performer;
	}

	Vehicle *Scene::createVehicle() {
		assert(vehicles_factory);
		if(!vehicles_factory) return nullptr;

		Vehicle* vehicle= vehicles_factory->createObject();

		vehicles.push_back(vehicle);

		return vehicle;
	}

	void Scene::setRunsFactory(SceneObjectsFactory<Run> *factory)
	{
		this->runs_factory = factory;
	}

	void Scene::setStopsFactory(SceneObjectsFactory<WorkStop> *factory)
	{
		this->stops_factory = factory;
	}

	void Scene::setRunVehicleSelectorsFactory(Factory<RunVehicleBinder> *factory) {
        this->run_vehicle_binders_factory = factory;
    }
}

