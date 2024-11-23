#ifndef LOGGER_H
#define LOGGER_H
#include <vector>
#include <string>

/**
 * Enum representing the type of log entry.
 */
enum LogType 
{
    LOG_INFO,      /**< Informational log entry */
    LOG_WARNING,   /**< Warning log entry */
    LOG_ERROR      /**< Error log entry */
};

/**
 * Struct representing a single log entry with a type and message.
 */
struct LogEntry 
{
    LogType type;                /**< Type of log entry */
    std::string message;         /**< Message content of the log entry */
};

/**
 * Logger class responsible for managing and outputting log messages.
 */
class Logger 
{
    public:
        /**
         * Static list of all log entries recorded.
         */
        static std::vector<LogEntry> messages;

        /**
         * Logs an informational message.
         *
         * @param message The message to log as an informational entry.
         */
        static void Log(const std::string& message);
        
        /**
        * Logs a warning message to the console in yellow color.
        *
        * @param message The warning message to log.
        */
        static void Warn(const std::string& message);

        /**
         * Logs an error message.
         *
         * @param message The message to log as an error entry.
         */
        static void Err(const std::string& message);

        /**
         * Static function from save log file in project  folder
         */
        static void SaveLogToFile();


private:
    static std::string filePath;
};

#endif
