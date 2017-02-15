#pragma once

#include <vector>
#include <cstddef>

namespace Scheduler
{
    class Run;
    class ScheduleCostFunction;
    class Cost;

    class TemperatureScheduler
    {
    public:
        virtual ~TemperatureScheduler()
        {
        }

        virtual float getTemperature() const = 0;
        virtual bool isFinish() const = 0;
        virtual const char* getName() const = 0;

        virtual void adapt (Cost delta, float random) = 0;
        virtual void changeTemperature() = 0;
        virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) = 0;

        virtual TemperatureScheduler* clone () const = 0;

    protected:
        std::vector<Cost> create_initial_costs(Run& run, const ScheduleCostFunction& cost_function, std::size_t length);
    };

    class DefaultTemperatureScheduler : public TemperatureScheduler
    {
    public:
        DefaultTemperatureScheduler(float p0, float p_end, std::size_t iterations_number);
        virtual float getTemperature() const override;
        virtual bool isFinish() const override;
        virtual const char* getName() const override;

        virtual void adapt (Cost delta, float random) override;
        virtual void changeTemperature() override;
        virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) override;

        virtual TemperatureScheduler* clone () const override;
    private:
        const float p0;
        const float p_end;
        std::size_t iterations_number;
        float current_temperature;
        float k;
    };
}
