#pragma once

#include <cstddef>
#include <vector>
#include <Engine/Utils/Factory.h>
#include "Extensions/ScheduleActualizationAlgorithm.h"

#include <SceneManager_export.h>

namespace Scheduler
{
    class Schedule;
    class Run;
    class Stop;
    class Operation;
    class Vehicle;
    class ScheduleActualizationAlgorithm;
    class ScheduleActualizationAlgorithmsFactory;

	/**
		Aggregator class for all schedule actualization algorithms
	*/
    class SCENEMANAGER_EXPORT ScheduleActualizer
    {
    public:
        ScheduleActualizer(Schedule* schedule);
		ScheduleActualizer(Schedule* schedule, const ScheduleActualizer &rhs);
        ~ScheduleActualizer();

        void onOperationAdded(const Stop* stop, const Operation* operation);
        void onOperationRemoved(const Stop* stop);
        void onStopNextRouteChanged(const Stop* stop);

        void onStopAdded(const Run* run, const Stop* stop, size_t index);
        void onStopRemoved(const Run* run, size_t index);
		void onStopReplaced(const Run* run, const Stop* new_stop, size_t index);
        void onRunVehicleChanged(const Run* run, const Vehicle* vehicle);

        void onRunAdded(const Run* run, size_t index);
        void onRunRemoved();

        void actualize();

        template<typename T, typename... Args>
        T* createAlgorithm(Args&&...args)
        {
            T* algorithm = algorithms_factory->createObject<T>(schedule, std::forward<Args>(args)...);
            if(!algorithm) return nullptr;
            algorithms.push_back(algorithm);
            return algorithm;
        };

		// == framework internal ====================================
		void setScheduleActualizationAlgorithmsFactory(Factory<ScheduleActualizationAlgorithm>* factory);

    private:
        Schedule* schedule;

        Factory<ScheduleActualizationAlgorithm>* algorithms_factory;
        std::vector<ScheduleActualizationAlgorithm*> algorithms;

		bool is_actualizing;
    };
}