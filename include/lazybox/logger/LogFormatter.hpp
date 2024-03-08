#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGFORMATTER_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGFORMATTER_HPP_

#include <thread>
#include <string>
#include <string_view>
#include <chrono>

#include "lazybox/logger/Basic.hpp"

namespace lbox {

struct LogHead {
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    std::thread::id thread_id;

    TimePoint time_stamp;

    LogLevel         level;
    std::string_view tag;

    std::string_view file_name;
    int              line_num;
    std::string_view func_name;

    LogHead(LogLevel         level,
            std::string_view tag,
            std::string_view file_name,
            int              line_num,
            std::string_view func_name) :
        thread_id(std::this_thread::get_id()), time_stamp(std::chrono::system_clock::now()), level(level), tag(tag),
        file_name(file_name), line_num(line_num), func_name(func_name) {}
};

#define MAKELOGHEAD(level, tag) (lbox::LogHead{level, tag, __FILE__, __LINE__, __func__})

class LogFormatter {
public:
    virtual std::string FormatLogHead(const LogHead &head)                  = 0;
    virtual std::string Format(const LogHead &head, const std::string &msg) = 0;
};

class DefaultLogFormatter : public LogFormatter {
public:
    std::string FormatLogHead(const LogHead &head) override;
    std::string Format(const LogHead &head, const std::string &msg) override;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGFORMATTER_HPP_
