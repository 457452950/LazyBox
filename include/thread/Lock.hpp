#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_LOCK_HPP_
#define LAZYBOX_INCLUDE_THREAD_LOCK_HPP_

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "base/Sysinfo.h"

#ifdef LBOX_WIN32
#include <Windows.h>
#endif

#include "Toy/NonCopyAble.hpp"

namespace lbox {

/**
 * 自旋锁
 */
class SpinLock final : public NonCopyAble {
public:
    SpinLock()  = default;
    ~SpinLock() = default;

    void Lock() {
        while(flag_.test_and_set()) {
        }
    }
    void Unlock() { flag_.clear(); }

    void lock() { Lock(); }
    void unlock() { Unlock(); }

private:
    std::atomic_flag flag_;
};

#ifdef LBOX_WIN32
// 用户态, 可重入锁, only for win32
class FastLock : public NonCopyAble {
public:
    FastLock() { InitializeCriticalSection(&cs_); }
    ~FastLock() { DeleteCriticalSection(&cs_); }

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
