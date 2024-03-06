#pragma once
#ifndef LAZYBOX_SRC_LOGGER_BASIC_HPP_
#define LAZYBOX_SRC_LOGGER_BASIC_HPP_

#include <unordered_set>
#include <string>
#include <memory>

namespace lbox {

enum LogLevel {
    L_DEBUG,
    L_INFO,
    L_WARNING,
    L_ERROR,
    L_FATAL,
};

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
