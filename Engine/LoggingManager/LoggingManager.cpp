#include "LoggingManager.h"
#include <spdlog/spdlog.h>

namespace Scheduler
{
	static spdlog::level::level_enum log_level = spdlog::level::debug;
	static std::vector<spdlog::sink_ptr> sinks;

	Logger& LoggingManager::getLogger(const char* name)
	{
		std::shared_ptr<spdlog::logger> logger = spdlog::get(name);
		if(!logger)
		{
			logger = std::make_shared<spdlog::logger>(name, sinks.begin( ), sinks.end( ));
			logger->set_level(log_level);
			spdlog::register_logger(logger);
		}
		return *logger.get( );
	}

	void LoggingManager::configure(const char* config_filename)
	{
		log_level = spdlog::level::trace;
		//sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
		sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("cpp_scheduler_log.txt"));
	}
}
