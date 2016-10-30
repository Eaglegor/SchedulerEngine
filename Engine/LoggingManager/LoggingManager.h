#pragma once
#include <spdlog/logger.h>
#include <LoggingManager_export.h>

namespace Scheduler
{
	using Logger = spdlog::logger;

	class LOGGINGMANAGER_EXPORT LoggingManager
	{
	public:
		static Logger& getLogger(const char* name);
		static void configure(const char* config_filename);
	};
	
#ifdef DEBUG_LOGGING
	static const bool DEBUG_LOGGING_ENABLED = true;
	
	#define LOG_ERROR(logger, ...) logger.error(__VA_ARGS__)
	#define LOG_WARNING(logger, ...) logger.warn(__VA_ARGS__)
	#define LOG_INFO(logger, ...) logger.info(__VA_ARGS__)
	#define LOG_DEBUG(logger, ...) logger.debug(__VA_ARGS__)
	#define LOG_TRACE(logger, ...) logger.trace(__VA_ARGS__)
	
	#define TRACEABLE_SECTION(name, title, logger) TraceableSection name(title, logger);
	
#else
	static const bool DEBUG_LOGGING_ENABLED = false;
	
	#define LOG_ERROR(logger, ...) logger.error(__VA_ARGS__)
	#define LOG_WARNING(logger, ...) logger.warn(__VA_ARGS__)
	#define LOG_INFO(logger, ...) logger.info(__VA_ARGS__)
	#define LOG_DEBUG(logger, ...) (void*)0;
	#define LOG_TRACE(logger, ...) (void*)0
	
	#define TRACEABLE_SECTION(name, title, logger) (void*)0;
#endif

	class TraceableSection
	{
	public:
		TraceableSection(const char* name, Logger& logger):
		name(name),
		logger(logger)
		{
			LOG_TRACE(logger, "Entering section: {}", name); 
		}
		
		~TraceableSection()
		{
			LOG_TRACE(logger, "Leaving section: {}", name);
		}
		
	private:
		const char* name;
		Logger& logger;
	};	

}

