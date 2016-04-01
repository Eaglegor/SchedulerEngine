#pragma once

#include "BenchmarkResult.h"
#include "BenchmarkPublisher.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

namespace Scheduler
{
	class MarkdownBenchmarkPublisher : public BenchmarkPublisher
	{
	public:
		MarkdownBenchmarkPublisher(const std::string& output_filename) :
			output_filename(output_filename)
		{
			
		}

		void addResult(const BenchmarkResult& result)
		{
			auto iter = results.find(result.dataset_name);
			if(iter == results.end()) results.emplace(result.dataset_name, std::unordered_map<std::string, BenchmarkResult>());
			iter = results.find(result.dataset_name);

			iter->second.emplace(result.algorithm_name, result);
			optimal_costs.emplace(result.dataset_name, result.optimal_cost);
		}

		void publish()
		{
			std::vector<std::string> algorithm_names;

			for(const auto &dsiter : results)
			{
				for (const auto &resiter : dsiter.second)
				{
					if (std::find(algorithm_names.begin(), algorithm_names.end(), resiter.first) == algorithm_names.end())
					{
						algorithm_names.push_back(resiter.first);
					}
				}
			}

			std::ofstream file;
			file.open(output_filename);

			file << "### Cost results: ###" << std::endl << std::endl;
			file << "|\tDataset\t|\t";
			file << "Optimal\t|\t";

			for(const std::string &alg_name : algorithm_names)
			{
				file << alg_name << "\t|\t";
			}

			file << std::endl;

			file << "|\t---\t|\t---\t|\t";

			for (const std::string &alg_name : algorithm_names)
			{
				file << "---" << "\t|\t";
			}

			file << std::endl;

			for (const auto &dsiter : results)
			{
				file << "|\t" << dsiter.first << "\t|\t" << optimal_costs.at(dsiter.first) << "\t|\t";
				for (const std::string &alg_name : algorithm_names)
				{
					const BenchmarkResult& res = dsiter.second.at(alg_name);
					float deviation = (res.cost - res.optimal_cost) / res.optimal_cost * 100;
					file << res.cost << " (" << deviation << "%) " << "\t|\t";
				}
				file << std::endl;
			}

			file << std::endl;
			file << std::endl;

			file << "### Timing: ###" << std::endl << std::endl;
			file << "|\tDataset\t|\t";

			for (const std::string &alg_name : algorithm_names)
			{
				file << alg_name << "\t|\t";
			}

			file << std::endl;

			file << "|\t---\t|\t";

			for (const std::string &alg_name : algorithm_names)
			{
				file << "---" << "\t|\t";
			}

			file << std::endl;

			for (const auto &dsiter : results)
			{
				file << "|\t" << dsiter.first << "\t|\t";
				for (const std::string &alg_name : algorithm_names)
				{
					const BenchmarkResult& res = dsiter.second.at(alg_name);
					file << res.average_time << "\t|\t";
				}
				file << std::endl;
			}


			file.close();
		}

	private:
		std::unordered_map<std::string, float> optimal_costs;
		std::unordered_map<std::string, std::unordered_map<std::string, BenchmarkResult>> results;
		std::string output_filename;
	};
}