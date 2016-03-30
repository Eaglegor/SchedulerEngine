#pragma once

namespace Scheduler
{
    class TemperatureFunction
    {
    public:
        virtual ~TemperatureFunction()
        {
        }

        virtual float getTemperature() const = 0;
        virtual bool isFinish() const = 0;

        virtual void changeTemperature() = 0;
        virtual void reset() = 0;

        virtual const char* getName() const = 0;
    };
}
