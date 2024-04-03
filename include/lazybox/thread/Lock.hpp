#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_LOCK_HPP_
#define LAZYBOX_INCLUDE_THREAD_LOCK_HPP_

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "lazybox/base/Sysinfo.h"

#ifdef LBOX_WIN32
#include <Windows.h>
#endif

#include "lazybox/Toy/NonCopyAble.hpp"

namespace lbox {

/**
 * 自旋锁
 */
class SpinLock final {
public:
    SpinLock()  = default;
    ~SpinLock() = default;

    NON_COPYABLE_(SpinLock)

    void Lock() {
        while(flag_.test_and_set(std::memory_order_acquire))
            ;
    }
    void Unlock() { flag_.clear(std::memory_order_release); }

    void lock() { Lock(); }
    void unlock() { Unlock(); }

private:
    std::atomic_flag flag_{};
};

#ifdef LBOX_WIN32
// 用户态, 可重入锁, only for win32
class FastLock final {
public:
    FastLock() { InitializeCriticalSection(&cs_); }
    ~FastLock() { DeleteCriticalSection(&cs_); }

    NON_COPYABLE_(FastLock)

    void Lock() { EnterCriticalSection(&cs_); }
    void Unlock() { LeaveCriticalSection(&cs_); }

    void lock() { Lock(); }
    void unlock() { Unlock(); }

private:
    CRITICAL_SECTION cs_{};
};
#elif defined(LBOX_LINUX)
using FastLock = SpinLock;
#endif


} // namespace lbox


#endif // LAZYBOX_INCLUDE_THREAD_LOCK_HPP_
