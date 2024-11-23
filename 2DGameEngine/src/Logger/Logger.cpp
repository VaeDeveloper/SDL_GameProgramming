#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream> 

ENGINE_API std::vector<LogEntry> Logger::messages;



/** log.txt file path from text file debug  */
ENGINE_API std::string Logger::filePath = "log.txt";

ENGINE_API void Logger::SaveLogToFile() 
{
    std::ofstream file(filePath, std::ios::trunc); 

    if (!file.is_open()) 
    {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    for (const auto& logEntry : messages) 
    {
        file << logEntry.message << std::endl;
    }

    file.close();
}

/**
 * Retrieves the current date and time as a formatted string.
 *
 * @return A string representing the current date and time in the format "DD-MMM-YYYY HH:MM:SS".
 */
ENGINE_API std::string CurrentDateTimeToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
    return output;
}

/**
 * Logs an informational message to the console in green color.
 *
 * @param message The informational message to log.
 */
ENGINE_API void Logger::Log(const std::string& message) 
{
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;

    messages.push_back(logEntry);
}

/**
 * Logs a warning message to the console in yellow color.
 *
 * @param message The warning message to log.
 */
ENGINE_API void Logger::Warn(const std::string& message) 
{
    LogEntry logEntry;
    logEntry.type = LOG_WARNING;
    logEntry.message = "WARN: [" + CurrentDateTimeToString() + "]: " + message;
    messages.push_back(logEntry);

    std::cout << "\x1B[33m" << logEntry.message << "\033[0m" << std::endl;
}

/**
 * Logs an error message to the console in red color.
 *
 * @param message The error message to log.
 */
ENGINE_API void Logger::Err(const std::string& message) 
{
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;

    messages.push_back(logEntry);
    std::cerr << "\x1B[91m"<< logEntry.message << "\033[0m" << std::endl;
}


