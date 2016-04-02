#pragma once

#include "BenchmarkResult.h"
#include "BenchmarkPublisher.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>

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
			for (const auto &kpi : result.kpi)
			{
				auto iter = results.find(kpi.first);

				if (iter == results.end()) results.emplace(kpi.first, std::map<std::string, std::map<std::string, std::string> >());
				iter = results.find(kpi.first);

				auto iter2 = iter->second.find(result.dataset_name);
				if (iter2 == iter->second.end()) iter->second.emplace(result.dataset_name, std::map<std::string, std::string>());
				iter2 = iter->second.find(result.dataset_name);

				iter2->second.emplace(result.algorithm_name, result.kpi.at(kpi.first));
			}
		}

		void publish()
		{
			std::ofstream file;
			file.open(output_filename);

			for (const auto &kpiter : results)
			{
				std::vector<std::string> algorithm_names;

				for (const auto &dsiter : kpiter.second)
				{
					for (const auto &resiter : dsiter.second)
					{
						if (std::find(algorithm_names.begin(), algorithm_names.end(), resiter.first) == algorithm_names.end())
						{
							algorithm_names.push_back(resiter.first);
						}
					}
				}
				
				std::sort(algorithm_names.begin(), algorithm_names.end());

				file << "### " << kpiter.first << "###" << std::endl << std::endl;

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

				for(const auto& dsiter : kpiter.second)
				{
					file << dsiter.first << "\t|\t";

					for (const std::string &alg_name : algorithm_names)
					{
						auto iter = dsiter.second.find(alg_name);
						if (iter == dsiter.second.end())
						{
							file << " - \t|\t";
						}
						else
						{
							const std::string& res = iter->second;
							file << res << "\t|\t";
						}
					}
					file << std::endl;
				}
			}

			file.close();
		}

	private:
		std::string output_filename;
		std::map < std::string, std::map<std::string, std::map<std::string, std::string> > > results;
	};
}