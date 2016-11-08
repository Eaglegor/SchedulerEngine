#pragma once

#include "TemperatureScheduler.h"
#include <queue>
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>

namespace Scheduler
{
    class Cost;

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT ListTemperatureScheduler : public TemperatureScheduler
    {
    public:
        ListTemperatureScheduler ();
        ListTemperatureScheduler (std::size_t values_length, float initialProbability, std::size_t maxIterationsCount);
        virtual ListTemperatureScheduler* clone () const override;
        virtual float getTemperature() const override;
        virtual bool isFinish() const override;

        virtual void adapt(Cost delta, float random) override;
        virtual void changeTemperature() override;
        virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) override;

        static constexpr const char* staticGetName() { return "List"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }

    protected:
        virtual float new_value();

        std::vector<float> adapt_values;
    private:
        const std::size_t values_length;
        const std::size_t max_iterations_number;
        const float initial_probability;
        std::priority_queue<float> values;
        std::size_t iteration_counter;
    };

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT FastTemperatureScheduler : public ListTemperatureScheduler
    {
    public:
        FastTemperatureScheduler ();
        FastTemperatureScheduler (std::size_t values_length, float initialProbability, std::size_t maxIterationsCount, float quality = 0.f);
        virtual FastTemperatureScheduler* clone () const override;
    protected:
        virtual float new_value() override;
    private:
        const float first_ratio;
    };

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT SlowTemperatureScheduler : public ListTemperatureScheduler
    {
    public:
        SlowTemperatureScheduler ();
        SlowTemperatureScheduler (std::size_t values_length, float initialProbability, std::size_t maxIterationsCount, float speed = 0.f);
        virtual SlowTemperatureScheduler* clone () const override;
    protected:
        virtual float new_value() override;
    private:
        const float first_ratio;
    };
}
