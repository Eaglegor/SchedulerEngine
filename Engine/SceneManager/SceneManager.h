#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Attribute.h"
#include "ConceptFactory.h"

#include "Factory.h"

namespace Scheduler
{
    class Scene;
    class Operation;
    class Order;
    class Vehicle;
    class Performer;
    class Schedule;
	class Run;
	class Stop;
    class ScheduleActualizationAlgorithm;

	class RoutingService;
	class LoggingService;

    class SceneManager
    {
    public:
		SceneManager();
        ~SceneManager();

        Scene* createScene();
        void destroyScene(Scene *scene);

        const Attribute*createAttribute(const char *name);

		void setRoutingService(RoutingService *routing_service);

    private:
        ConceptFactory<Scene> scenes_factory;
        ConceptFactory<Operation> operations_factory;
        ConceptFactory<Order> orders_factory;
        ConceptFactory<Vehicle> vehicles_factory;
        ConceptFactory<Performer> performers_factory;
        ConceptFactory<Schedule> schedules_factory;
        ConceptFactory<Attribute> attributes_factory;
		ConceptFactory<Run> runs_factory;
		ConceptFactory<Stop> stops_factory;

		Factory<ScheduleActualizationAlgorithm> schedule_actualization_algorithms_factory;

        std::unordered_set<Scene*> scenes;
        std::unordered_map<std::string, Attribute*> attributes;

		RoutingService* routing_service;
    };
}