#pragma once
#ifndef LAZYBOX_I_INCLUDE_TIME_H_
#define LAZYBOX_I_INCLUDE_TIME_H_

#include <chrono>

namespace lbox {

inline long long GetTick64() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

} // namespace lbox

#endif // LAZYBOX_I_INCLUDE_TIME_H_
