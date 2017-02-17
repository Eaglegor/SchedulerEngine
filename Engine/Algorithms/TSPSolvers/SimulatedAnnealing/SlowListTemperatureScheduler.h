#pragma once

#include "ListTemperatureScheduler.h"

namespace Scheduler
{

class SIMULATEDANNEALINGTSPSOLVER_EXPORT SlowListTemperatureScheduler : public ListTemperatureScheduler
{
public:
    SlowListTemperatureScheduler();
    SlowListTemperatureScheduler(std::size_t list_size, std::size_t max_iterations_count, float initial_probability, float speed = 0.f);
    virtual SlowListTemperatureScheduler* clone() const override;
    static constexpr const char* staticGetName() { return "List-Slow"; }
    virtual const char* getName() const override
    {
        return staticGetName();
    }
protected:
    virtual float new_value() override;
private:
    const float first_ratio;
};

}
