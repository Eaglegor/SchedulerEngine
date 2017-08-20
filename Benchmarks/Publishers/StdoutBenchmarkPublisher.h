#pragma once

#include "BenchmarkPublisher.h"
#include "BenchmarkResult.h"
#include <vector>

namespace Scheduler
{
	class StdoutBenchmarkPublisher : public BenchmarkPublisher
	{
	public:
		void addResult(const BenchmarkResult& result)
		{
			results.push_back(result);
		}

		void publish( )
		{
			for(const BenchmarkResult& result : results)
			{
				std::cout << std::endl;
				std::cout << "#### Benchmark: " << result.dataset_name << " (" << result.algorithm_name << ") ####" << std::endl;
				for(const auto& kpi : result.kpi)
				{
					std::cout << "# " << kpi.first << ": " << kpi.second << std::endl;
				}
				//std::cout << "# Cost: " << result.cost << std::endl;
				/*if (result.optimal_cost > 0)
					{
						std::cout << "# Optimal cost: " << result.optimal_cost << std::endl;
						std::cout << "# Deviation from optimal cost: " << ((result.cost - result.optimal_cost) / result.optimal_cost) * 100 << " %" << std::endl;
					}*/
				//std::cout << "#" << std::endl;
				//std::cout << "# Average time: " << result.average_time << " ms" << std::endl;
				std::cout << "#### End of benchmark ####" << std::endl;
				std::cout << std::endl;
			}
		}

	private:
		std::vector<BenchmarkResult> results;
	};
}