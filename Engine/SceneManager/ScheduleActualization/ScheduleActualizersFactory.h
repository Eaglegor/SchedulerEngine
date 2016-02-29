#pragma once

namespace Scheduler
{
    class ScheduleActualizationAlgorithmsFactory;
    class ScheduleActualizer;
    class Schedule;

    class ScheduleActualizersFactory
    {
    public:
        ScheduleActualizersFactory(ScheduleActualizationAlgorithmsFactory* algorithms_factory);

        ScheduleActualizer* createScheduleActualizer(Schedule* schedule);
        void destroyScheduleActualizer(ScheduleActualizer* actualizer);

    private:
        ScheduleActualizationAlgorithmsFactory* algorithms_factory;
    };
}