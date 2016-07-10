#pragma once
#include <spdlog/logger.h>
#include <LoggingManager_export.h>

namespace Scheduler
{
	using Logger = spdlog::logger;

	class LOGGINGMANAGER_EXPORT LoggingManager
	{
	public:
		static Logger* getLogger(const char* name);
	};
}

#define LOG_TRACE(logger, ...) if(logger) logger->trace(__VA_ARGS__)
#define LOG_DEBUG(logger, ...) if(logger) logger->debug(__VA_ARGS__)
#define LOG_INFO(logger, ...) if(logger) logger->info(__VA_ARGS__)
#define LOG_WARNING(logger, ...) if(logger) logger->warn(__VA_ARGS__)
#define LOG_ERROR(logger, ...) if(logger) logger->error(__VA_ARGS__)
#define LOG_CRITICAL(logger, ...) if(logger) logger->error(__VA_ARGS__)

#define SIMPLE_LOG_TRACE(logger, message) if(logger) logger->trace<const char*>(message)
#define SIMPLE_LOG_DEBUG(logger, message) if(logger) logger->debug<const char*>(message)
#define SIMPLE_LOG_INFO(logger, message) if(logger) logger->info<const char*>(message)
#define SIMPLE_LOG_WARNING(logger, message) if(logger) logger->warn<const char*>(message)
#define SIMPLE_LOG_ERROR(logger, message) if(logger) logger->error<const char*>(message)
#define SIMPLE_LOG_CRITICAL(logger, message) if(logger) logger->error<const char*>(message)