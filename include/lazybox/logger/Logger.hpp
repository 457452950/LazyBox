#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_

#include <iostream>
#include <unordered_set>

#include "lazybox/fmt/Format.h"
#include "lazybox/toy/Instance.hpp"

#include "Basic.hpp"
#include "LogFormatter.hpp"
#include "LogWriter.hpp"

namespace lbox {


class Logger : public Instance<Logger> {
public:
    Logger()  = default;
    ~Logger() = default;

    Logger *SetConfig(const LogConfig &config);

    Logger *AddFileLogger(const std::filesystem::path &output_path);
    Logger *SetSTDLogger(bool enable = true);

    bool     CheckTag(const std::string &tag) const;
    LogLevel Level() const;

    void Write(LogLevel level, const std::string &tag, const std::string &message);

    void Stop();

private:
    LogConfig                                config_;
    std::vector<std::shared_ptr<FileWriter>> loggers_;
    std::shared_ptr<LogWriter>               std_writer_{nullptr};
};

namespace {
void _check_compile() {}

} // namespace

} // namespace lbox

#endif // LAZYBOX_INCLUDE_LAZYBOX_LOGGER_LOGGER_HPP_
