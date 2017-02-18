#include "SlowListTemperatureScheduler.h"
#include <numeric>
#include <algorithm>

namespace Scheduler
{

SlowListTemperatureScheduler::SlowListTemperatureScheduler() : ListTemperatureScheduler(), first_ratio(0.f)
{}

SlowListTemperatureScheduler::SlowListTemperatureScheduler(std::size_t length, std::size_t iterations, float probability, float speed) :
    ListTemperatureScheduler(length, iterations, probability), first_ratio(speed)
{}

float SlowListTemperatureScheduler::new_value()
{
    if (adapt_values.empty()) {
        return 0.f;
    }

    std::size_t ofs = first_ratio * adapt_values.size();
    ofs = std::min(adapt_values.size(), ofs);
    ofs = std::max(std::size_t(1), ofs);

    std::partial_sort(adapt_values.begin(), std::next(adapt_values.begin(), ofs), adapt_values.end(), std::greater<float>());
    const float new_value = std::accumulate(adapt_values.begin(), std::next(adapt_values.begin(), ofs), 0.f) / ofs;
    return new_value;
}

SlowListTemperatureScheduler* SlowListTemperatureScheduler::clone() const
{
    return new SlowListTemperatureScheduler(*this);
}

}
