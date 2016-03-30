#include "BasicAcceptanceFunction.h"
#include <algorithm>
#include <cstdlib>

namespace Scheduler
{
    bool BasicAcceptanceFunction::isAccepting(const Cost &delta, float temperature) const
    {
        const float normalized_value = std::exp(-delta.getValue() / temperature);
        const int rand_value = std::rand();
        const float normalized_rand = static_cast<float>(rand_value) / static_cast<float>(RAND_MAX);
        return normalized_rand <= normalized_value;
    }
}
