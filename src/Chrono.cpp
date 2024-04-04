#include "lazybox/Chrono.h"

#include "lazybox/logger/Logger.hpp"

namespace lbox {

auto GetTickMs64() -> int64_t {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

auto GetTickUs64() -> int64_t {
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

TimeCostCounter::TimeCostCounter(std::string tag) : tag_(std::move(tag)) {}

TimeCostCounter::~TimeCostCounter() { this->LogProcess(); }

void TimeCostCounter::LogProcess() { lbox::println("process [{}] cost {} us", tag_, GetTickUs64() - start_time_us_); }

} // namespace lbox