#pragma once

#include <string>
#include <unordered_map>

namespace Scheduler
{
    struct BenchmarkResult
    {
        std::string dataset_name;
        std::string algorithm_name;
        
		std::unordered_map<std::string, std::string> kpi;
	};
}