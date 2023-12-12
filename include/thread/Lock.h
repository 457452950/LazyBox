#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_LOCK_H_
#define LAZYBOX_INCLUDE_THREAD_LOCK_H_

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "base/Sysinfo.h"


#ifdef LBOX_WIN32
#include <Windows.h>
#endif

namespace lbox {

/**
 * 自旋锁
 */
class SpinLock final {
public:
    SpinLock()  = default;
    ~SpinLock() = default;

    // no copy able
    SpinLock &operator=(const SpinLock &) = delete;
    SpinLock(const SpinLock &)            = delete;

    void Lock() {
        while(flag_.test_and_set()) {
        }
    }
    void Unlock() { flag_.clear(); }

private:
    std::atomic_flag flag_;
};

template <class T>
class UniqueLock {
public:
    explicit UniqueLock(T &lock) : lock_(lock) { lock_.Lock(); }
    ~UniqueLock() { lock_.Unlock(); }

    UniqueLock(const UniqueLock &)            = delete;
    UniqueLock &operator=(const UniqueLock &) = delete;

private:
    T &lock_;
};

#ifdef LBOX_WIN32
// 用户态, 可重入锁, only for win32
class FastLock {
public:
    FastLock() { InitializeCriticalSection(&cs_); }
    ~FastLock() { DeleteCriticalSection(&cs_); }

    FastLock(const FastLock &)            = delete;
    FastLock &operator=(const FastLock &) = delete;

    void Lock() { EnterCriticalSection(&cs_); }
    void Unlock() { LeaveCriticalSection(&cs_); }

private:
    CRITICAL_SECTION cs_{};
};

#elif defined(LBOX_LINUX)
using FastLock = SpinLock;
#endif


} // namespace lbox


#endif // LAZYBOX_INCLUDE_THREAD_LOCK_H_
