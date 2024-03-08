#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_

#include <iostream>
#include <unordered_set>

#include "lazybox/fmt/Format.h"
#include "lazybox/toy/Instance.hpp"

#include "Basic.hpp"
#include "LogFormatter.hpp"
#include "LogReporter.hpp"

namespace lbox {

class LogHelper;

class Logger : public Instance<Logger> {
    friend class LogHelper;

public:
    Logger();
    ~Logger() = default;

    Logger *SetConfig(const LogConfig &config);

    Logger *SetSTDLogger(bool enable = true);
    Logger *AddFileLogger(const std::filesystem::path &output_path, bool async = false);
    Logger *AddReporter(std::shared_ptr<LogReporter> reporter);

    Logger *SetFormmater(std::shared_ptr<LogFormatter> formatter);

    bool     CheckTag(const std::string &tag) const;
    LogLevel Level() const;

    void Write(LogLevel level, const std::string &tag, const std::string &message);

    void Stop();

private:
    std::atomic_bool                          active_{true};
    LogConfig                                 config_;
    std::vector<std::shared_ptr<LogReporter>> loggers_;
    std::shared_ptr<LogReporter>              std_writer_{nullptr};
    std::shared_ptr<LogFormatter>             formatter_{nullptr};
};

class LogHelper final {
public:
    template <typename... Args>
    static void Commit(const LogHead &head, const lbox::format_string<Args...> fmt, Args &&...args) {
        auto log = Logger::GetInstance();
        auto tag = to_string(head.tag);

        if(!log->active_.load(std::memory_order_acquire)) {
            return;
        }

        if(log->CheckTag(tag) && log->Level() <= head.level) {
            log->Write(head.level,
                       tag,
                       log->formatter_->Format(head, lbox::vformat(fmt, lbox::make_format_args(args...))));
        }
    }
};

} // namespace lbox

#define LOG_(level, tag, ...)                                                                                          \
    lbox::LogHelper::Commit(MAKELOGHEAD(lbox::LogLevel::L_##level, tag) __VA_OPT__(, ) __VA_ARGS__)

#define LOG_FAT(tag, ...) LOG_(FATAL, tag, __VA_ARGS__)
#define LOG_ERR(tag, ...) LOG_(ERROR, tag, __VA_ARGS__)
#define LOG_WAR(tag, ...) LOG_(WARN, tag, __VA_ARGS__)
#define LOG_INF(tag, ...) LOG_(INFO, tag, __VA_ARGS__)
#define LOG_DBG(tag, ...) LOG_(DEBUG, tag, __VA_ARGS__)

namespace {
void _check_compile() {
    LOG_DBG("abc", "hello");
    LOG_DBG("abc", "hello {}", "world");
    LOG_INF("abc", "hello");
    LOG_INF("abc", "hello {}", "world");
    LOG_WAR("abc", "hello");
    LOG_WAR("abc", "hello {}", "world");
    LOG_ERR("abc", "hello");
    LOG_ERR("abc", "hello {}", "world");
    LOG_FAT("abc", "hello");
    LOG_FAT("abc", "hello {}", "world");
}
} // namespace

#endif // LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_
