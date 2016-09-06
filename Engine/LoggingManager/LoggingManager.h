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
		static void configure(const char* config_filename);
	};
}

#ifdef DEBUG_LOGGING
	#define LOG_TRACE(logger, ...) if(logger) logger->trace(__VA_ARGS__)
	#define LOG_DEBUG(logger, ...) if(logger) logger->debug(__VA_ARGS__)
#else
	#define LOG_TRACE(logger, ...) (void*)(0)
	#define LOG_DEBUG(logger, ...) (void*)(0)
#endif

#define LOG_INFO(logger, ...) if(logger) logger->info(__VA_ARGS__)
#define LOG_WARNING(logger, ...) if(logger) logger->warn(__VA_ARGS__)
#define LOG_ERROR(logger, ...) if(logger) logger->error(__VA_ARGS__)
#define LOG_CRITICAL(logger, ...) if(logger) logger->error(__VA_ARGS__)

#ifdef DEBUG_LOGGING
	#define SIMPLE_LOG_TRACE(logger, message) if(logger) logger->trace<const char*>(message)
	#define SIMPLE_LOG_DEBUG(logger, message) if(logger) logger->debug<const char*>(message)
#else
	#define SIMPLE_LOG_TRACE(logger, message) (void*)(0)
	#define SIMPLE_LOG_DEBUG(logger, message) (void*)(0)
#endif

#define SIMPLE_LOG_INFO(logger, message) if(logger) logger->info<const char*>(message)
#define SIMPLE_LOG_WARNING(logger, message) if(logger) logger->warn<const char*>(message)
#define SIMPLE_LOG_ERROR(logger, message) if(logger) logger->error<const char*>(message)
#define SIMPLE_LOG_CRITICAL(logger, message) if(logger) logger->error<const char*>(message)


#ifdef DEBUG_LOGGING
	#define STATIC_LOG_TRACE(name, ...) static Logger* logger = LoggingManager::getLogger(#name); LOG_TRACE(logger, __VA_ARGS__)
	#define STATIC_LOG_DEBUG(name, ...) static Logger* logger = LoggingManager::getLogger(#name); LOG_DEBUG(logger, __VA_ARGS__)
#else
	#define STATIC_LOG_TRACE(name, ...) (void*)(0)
	#define STATIC_LOG_DEBUG(name, ...) (void*)(0)
#endif

#define STATIC_LOG_INFO(name, ...)  static Logger* logger = LoggingManager::getLogger(#name); LOG_INFO(logger, __VA_ARGS__)
#define STATIC_LOG_WARNING(name, ...) static Logger* logger = LoggingManager::getLogger(#name); LOG_WARNING(logger, __VA_ARGS__)
#define STATIC_LOG_ERROR(name, ...) static Logger* logger = LoggingManager::getLogger(#name); LOG_ERROR(logger, __VA_ARGS__)
#define STATIC_LOG_CRITICAL(name, ...) static Logger* logger = LoggingManager::getLogger(#name); LOG_CRITICAL(logger, __VA_ARGS__)

#ifdef DEBUG_LOGGING
	#define STATIC_SIMPLE_LOG_TRACE(name, message) static Logger* logger = LoggingManager::getLogger(#name); LOG_TRACE(logger, message)
	#define STATIC_SIMPLE_LOG_DEBUG(name, message) static Logger* logger = LoggingManager::getLogger(#name); LOG_DEBUG(logger, message)
#else
	#define STATIC_SIMPLE_LOG_TRACE(name, message) (void*)(0)
	#define STATIC_SIMPLE_LOG_DEBUG(name, message) (void*)(0)
#endif

#define STATIC_SIMPLE_LOG_INFO(name, message) static Logger* logger = LoggingManager::getLogger(#name); LOG_INFO(logger, message)
#define STATIC_SIMPLE_LOG_WARNING(name, message) static Logger* logger = LoggingManager::getLogger(#name); LOG_WARNING(logger, message)
#define STATIC_SIMPLE_LOG_ERROR(name, message) static Logger* logger = LoggingManager::getLogger(#name); LOG_ERROR(logger, message)
#define STATIC_SIMPLE_LOG_CRITICAL(name, message) static Logger* logger = LoggingManager::getLogger(#name); LOG_CRITICAL(logger, message)