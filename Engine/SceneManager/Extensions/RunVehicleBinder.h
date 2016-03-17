#pragma once

namespace Scheduler
{
    class Vehicle;
    class Run;

	/// Is used by the scene to automatically assign vehicles to the created runs
    class RunVehicleBinder
    {
    public:
        virtual ~RunVehicleBinder(){}

        virtual bool bindVehicle(Run* run) = 0;
    };
}