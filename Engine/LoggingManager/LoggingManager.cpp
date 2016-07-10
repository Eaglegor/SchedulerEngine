#include "LoggingManager.h"
#include <spdlog/spdlog.h>

namespace Scheduler
{
	Logger* LoggingManager::getLogger(const char* name)
	{
		std::shared_ptr<spdlog::logger> logger = spdlog::get(name);
		if (!logger)
		{
			logger = spdlog::stdout_logger_st(name);
			logger->set_level(spdlog::level::trace);
		}
		return logger.get();
	}
}
