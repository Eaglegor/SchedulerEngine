#pragma once

#include <spdlog/logger.h>

namespace Scheduler
{
	/** 
	 * @brief The type representing logger
	 * 
	 * @note spdlog::logger is used as the backend implementation
	 * 
	 */
	using Logger = spdlog::logger;

	/** 
	 * @brief This class provides loggers and manages their lifetime
	 */
	class LoggingManager
	{
	public:
		/** 
		 * @brief Retrieves the logger with the specified name
		 * 
		 * @param name The name of the logger to retrieve
		 * 
		 * @return The reference to the requested logger
		 */
		static Logger& getLogger(const char* name);

		/** 
		 * @brief Configures the logging system using the specified config file
		 * 
		 * @note This method must be called before requesting any loggers to initialize
		 * the logging system. Config file isn't used in the current implementation so any
		 * value may be passed as a parameter
		 * 
		 * @param config_filename The name of config file to configure the logging system from
		 */
		static void configure(const char* config_filename);
	};

#ifdef DEBUG_LOGGING
	/** 
	 * @brief True, if debug logging is enabled
	 */
	static const bool DEBUG_LOGGING_ENABLED = true;

/// Logs the error message using the specified logger
#define LOG_ERROR(logger, ...) logger.error(__VA_ARGS__)

/// Logs the warning message using the specified logger
#define LOG_WARNING(logger, ...) logger.warn(__VA_ARGS__)

/// Logs the info message using the specified logger
#define LOG_INFO(logger, ...) logger.info(__VA_ARGS__)

/// Logs the debug message using the specified logger.  Does nothing when debug logging is disabled
#define LOG_DEBUG(logger, ...) logger.debug(__VA_ARGS__)

/// Logs the trace message using the specified logger.  Does nothing when debug logging is disabled
#define LOG_TRACE(logger, ...) logger.trace(__VA_ARGS__)

/** 
 * @brief Starts a traceable section
 * 
 * @details Traceable section is a section of code for which the entrance and leaving is logged.
 * When this macro is called the RAII variable is created and the entrance message containing the section title is logged 
 * using the specified logger. When the control flow exits current scope, the destructor of the variable is called and 
 * the exit message containing the section title is logged using the specified logger.
 * 
 * @note Does nothing if debug logging is disabled
 * 
 * @param name The name of the variable
 * @param title The title of traceable section
 * @param logger The logger to be used to trace the messages
  * 
 */
#define TRACEABLE_SECTION(name, title, logger) TraceableSection name(title, logger);

#else
	/** 
	 * @brief True, if debug logging is enabled
	 */
	static const bool DEBUG_LOGGING_ENABLED = false;

/// Logs the error message using the specified logger
#define LOG_ERROR(logger, ...) logger.error(__VA_ARGS__)

/// Logs the warning message using the specified logger
#define LOG_WARNING(logger, ...) logger.warn(__VA_ARGS__)

/// Logs the info message using the specified logger
#define LOG_INFO(logger, ...) logger.info(__VA_ARGS__)

/// Logs the debug message using the specified logger. Does nothing when debug logging is disabled
#define LOG_DEBUG(logger, ...) (void*) 0;

/// Logs the trace message using the specified logger. Does nothing when debug logging is disabled
#define LOG_TRACE(logger, ...) (void*) 0

/** 
 * @brief Starts a traceable section
 * 
 * @details Traceable section is a section of code for which the entrance and leaving is logged.
 * When this macro is called the RAII variable is created and the entrance message containing the section title is logged 
 * using the specified logger. When the control flow exits current scope, the destructor of the variable is called and 
 * the exit message containing the section title is logged using the specified logger.
 * 
 * @note Does nothing if debug logging is disabled
 * 
 * @param name The name of the variable
 * @param title The title of traceable section
 * @param logger The logger to be used to trace the messages
  * 
 */
#define TRACEABLE_SECTION(name, title, logger) (void*) 0;
#endif

	/** 
	 * @brief Represents RAII object used to notify logger about the entrance and leaving of the code section starting after 
	 * the call to TRACEABLE_SECTION() and ending when the current scope ends.
	 */
	class TraceableSection
	{
	public:
		/** 
		 * @brief Constructor
		 * 
		 * @details When the constructor is called the entrance message is sent to the logger
		 * 
		 * @param name The name of the section
		 * @param logger The logger to send the message to
		 */
		TraceableSection(const char* name, Logger& logger)
		    : name(name),
		      logger(logger)
		{
			LOG_TRACE(logger, "Entering section: {}", name);
		}

		/** 
		 * @brief Destructor
		 * 
		 * @details When the destructor is called the leaving message is sent to the logger
		 */
		~TraceableSection( )
		{
			LOG_TRACE(logger, "Leaving section: {}", name);
		}

	private:
		const char* name;
		Logger& logger;
	};
}
