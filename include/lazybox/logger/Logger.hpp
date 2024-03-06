#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_

#include "lazybox/fmt/Format.h"

namespace lbox {

enum LogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
};

struct LogEntry {
    LogLevel    level;
    std::string message;
};

struct LoggerConfig {
    std::string log_file;
    LogLevel    log_level;
};

class Logger {};

class LogFormatter {};

class LogWriter {};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_
