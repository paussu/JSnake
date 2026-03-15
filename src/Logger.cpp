#include "Logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace
{
#ifdef NDEBUG
LogLevel gMinimumLogLevel = LogLevel::Error;
#else
LogLevel gMinimumLogLevel = LogLevel::Debug;
#endif

std::mutex gLoggerMutex;

bool ShouldLog(LogLevel level)
{
    return static_cast<int>(level) >= static_cast<int>(gMinimumLogLevel);
}

const char* ToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Error:
            return "ERROR";
        default:
            return "LOG";
    }
}
}

void Logger::SetMinimumLevel(LogLevel level)
{
    std::scoped_lock lock(gLoggerMutex);
    gMinimumLogLevel = level;
}

void Logger::Debug(std::string_view message, const std::source_location& location)
{
    Log(LogLevel::Debug, message, location);
}

void Logger::Error(std::string_view message, const std::source_location& location)
{
    Log(LogLevel::Error, message, location);
}

void Logger::Log(LogLevel level, std::string_view message, const std::source_location& location)
{
    if (!ShouldLog(level))
        return;

    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::scoped_lock lock(gLoggerMutex);
    std::clog << '[' << std::put_time(&localTime, "%H:%M:%S") << "] "
              << '[' << ToString(level) << "] "
              << '[' << location.function_name() << ':' << location.line() << "] "
              << message << std::endl;
}