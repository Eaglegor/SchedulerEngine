#pragma once

#include <string>

namespace Scheduler
{
    struct BenchmarkResult
    {
        std::string dataset_name;
        std::string algorithm_name;
        float cost;
        float optimal_cost;
        double average_time;
	};
}