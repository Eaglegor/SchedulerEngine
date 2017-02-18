#include "FastListTemperatureScheduler.h"
#include <numeric>
#include <algorithm>

namespace Scheduler
{
FastListTemperatureScheduler::FastListTemperatureScheduler() : ListTemperatureScheduler(), first_ratio(1.f)
{}

FastListTemperatureScheduler::FastListTemperatureScheduler(std::size_t list_size, std::size_t iterations_limit, float initial_probability, float quality) :
    ListTemperatureScheduler(list_size, iterations_limit, initial_probability), first_ratio(quality)
{}

float FastListTemperatureScheduler::new_value()
{
    if (adapt_values.empty()) {
        return 0.f;
    }

    std::size_t ofs = first_ratio * adapt_values.size();
    ofs = std::min(adapt_values.size(), ofs);
    ofs = std::max(std::size_t(1), ofs);
    std::partial_sort(adapt_values.begin(), std::next(adapt_values.begin(), ofs), adapt_values.end());
    const float new_value = std::accumulate(adapt_values.begin(), std::next(adapt_values.begin(), ofs), 0.f) / ofs;
    return new_value;
}

FastListTemperatureScheduler* FastListTemperatureScheduler::clone() const
{
    return new FastListTemperatureScheduler(*this);
}

}
