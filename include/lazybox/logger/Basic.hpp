#pragma once
#ifndef LAZYBOX_SRC_LOGGER_BASIC_HPP_
#define LAZYBOX_SRC_LOGGER_BASIC_HPP_

#include <unordered_set>
#include <string>
#include <memory>

namespace lbox {

enum LogLevel : int16_t {
    L_VERBOSE,
    L_DEBUG,
    L_INFO,
    L_WARN,
    L_ERROR,
    L_FATAL,
};

inline const char *LogLevelToString(LogLevel level) {
    switch(level) {
    case L_VERBOSE:
        return "VERBOSE";
    case L_DEBUG:
        return "DEBUG";
    case L_INFO:
        return "INFO";
    case L_WARN:
        return "WARN";
    case L_ERROR:
        return "ERROR";
    case L_FATAL:
        return "FATAL";
    }
    return "UNKNOWN";
}

enum LogOutput { STDOUT = 0x01, FILE = 0x02 };

using TagSet = std::unordered_set<std::string>;

struct LogConfig {
    LogLevel                level{L_INFO};
    std::shared_ptr<TagSet> tags{nullptr};
};

struct LogEntry {
    LogLevel    level;
    std::string message;
};


} // namespace lbox

#endif // LAZYBOX_SRC_LOGGER_BASIC_HPP_
