#include "lazybox/logger/LogFormatter.hpp"

#include "lazybox/fmt/Format.h"

namespace lbox {

std::string DefaultLogFormatter::FormatLogHead(const LogHead &head) {
    auto china_time = head.time_stamp + std::chrono::hours(8);
    return lbox::format("[{}][{}|{}][{}:{}:{}][tid:{}]",
                        china_time,
                        LogLevelToString(head.level),
                        head.tag,
                        head.file_name,
                        head.line_num,
                        head.func_name,
                        head.thread_id);
}

std::string DefaultLogFormatter::Format(const LogHead &head, const std::string &msg) {
    return lbox::format("{}:{}\n", FormatLogHead(head), msg);
}

} // namespace lbox