#pragma once

namespace Scheduler
{
    class Vehicle;
    class Run;

    class RunVehicleBinder
    {
    public:
        virtual ~RunVehicleBinder(){}

        virtual bool bindVehicle(Run* run) = 0;
    };
}