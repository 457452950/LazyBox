#ifndef LAZYBOX_INCLUDE_MEMORY_H_
#define LAZYBOX_INCLUDE_MEMORY_H_

#include "base/Sysinfo.h"

#include <mutex>

#ifdef LBOX_WIN32
#include <Windows.h>
#endif

namespace lbox{
#ifdef LBOX_WIN32

    // 用户态，可重入锁
    class FastLock {
    public:
        FastLock() {
            InitializeCriticalSection(&cs_);
        }
        ~FastLock() {
            DeleteCriticalSection(&cs_);
        }

        FastLock(const FastLock&) = delete;
        FastLock& operator=(const FastLock&) = delete;

        void Lock() {
            EnterCriticalSection(&cs_);
        }
        void Unlock() {
            LeaveCriticalSection(&cs_);
        }

    private:
        CRITICAL_SECTION cs_{};
    };

    class UniqueLock {
    public:
        explicit UniqueLock(FastLock & lock) : lock_(lock) {
            lock_.Lock();
        }
        ~UniqueLock() {
            lock_.Unlock();
        }

        UniqueLock(const UniqueLock&) = delete;
        UniqueLock& operator=(const UniqueLock&) = delete;

    private:
        FastLock& lock_;
    };

#endif
}


#endif//LAZYBOX_INCLUDE_MEMORY_H_
