#pragma once

#include <format>
#include <iostream>
#include <syncstream>
#include <atomic>

namespace BadSQL {
namespace Colors {
    constexpr const char* Red = "\x1B[91m";
    constexpr const char* Green = "\x1B[92m";
    constexpr const char* Blue = "\x1B[94m";
    constexpr const char* Yellow = "\x1B[93m";
    constexpr const char* White = "\x1B[97m";
    constexpr const char* Reset = "\x1B[0m";
} // namespace Colors

namespace Logger {
    enum class LogLevel {
        None,
        Errors,
        Warnings,
        Info,
        Debug
    };
    inline std::atomic<LogLevel> _logLevel = LogLevel::Errors;
    inline void setLogLevel(LogLevel lvl) { _logLevel = lvl; }

    template <typename... Args>
    static void _log(LogLevel lvl, std::string_view fmt, Args&&... args) {
        if (_logLevel == LogLevel::None)
            return;

        switch (lvl) {
            case LogLevel::Errors:
                if (lvl <= _logLevel)
                    std::osyncstream(std::cerr) << Colors::Red << "[ERROR]: " << Colors::Reset << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
                break;
            case LogLevel::Warnings:
                if (lvl <= _logLevel)
                    std::osyncstream(std::cerr) << Colors::Yellow << "[WARNING]: " << Colors::Reset << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
                break;
            case LogLevel::Info:
                if (lvl <= _logLevel)
                    std::osyncstream(std::cout) << Colors::Green << "[INFO]: " << Colors::Reset << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
                break;
            case LogLevel::Debug:
                if (lvl <= _logLevel)
                    std::osyncstream(std::cout) << Colors::Blue << "[DEBUG]: " << Colors::Reset << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
                break;
            default:
                break;
        }
    }

    template <typename... Args>
    void error(std::string_view fmt, Args&&... args) {
        _log(LogLevel::Errors, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void warn(std::string_view fmt, Args&&... args) {
        _log(LogLevel::Warnings, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void info(std::string_view fmt, Args&&... args) {
        _log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void debug(std::string_view fmt, Args&&... args) {
        _log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }
}
}
