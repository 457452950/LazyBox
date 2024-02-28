#pragma once
#ifndef LAZYBOX_INCLUDE_TIME_H_
#define LAZYBOX_INCLUDE_TIME_H_

#include <chrono>

namespace lbox {

inline auto GetTickMs64() -> int64_t {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

} // namespace lbox

#endif // LAZYBOX_INCLUDE_TIME_H_
