#pragma once

#include <source_location>
#include <string_view>

enum class LogLevel
{
    Debug,
    Error
};

class Logger
{
 public:
    static void SetMinimumLevel(LogLevel level);
    static void Debug(std::string_view message, const std::source_location& location = std::source_location::current());
    static void Error(std::string_view message, const std::source_location& location = std::source_location::current());

 private:
    static void Log(LogLevel level, std::string_view message, const std::source_location& location);
};