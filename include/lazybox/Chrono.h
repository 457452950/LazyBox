#pragma once
#ifndef LAZYBOX_INCLUDE_TIME_H_
#define LAZYBOX_INCLUDE_TIME_H_

#include <chrono>
#include <cstdint>

#include "base/DefineStrTool.h"

namespace lbox {

auto GetTickMs64() -> int64_t;

auto GetTickUs64() -> int64_t;

class TimeCostCounter final {
public:
    explicit TimeCostCounter(std::string tag);
    ~TimeCostCounter();

private:
    void LogProcess();

    int64_t     start_time_us_{GetTickUs64()};
    std::string tag_;
};


} // namespace lbox

#define FUNC_COST_COUNTER() lbox::TimeCostCounter SPLICE3(_TIME_COUNTER_, __COUNTER__, __LINE__)(__func__)

#define COST_COUNTER(TAG) lbox::TimeCostCounter __FILE__##__func__##__LINE__(TAG)

#endif // LAZYBOX_INCLUDE_TIME_H_
