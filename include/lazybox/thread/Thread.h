#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_THREAD_H_
#define LAZYBOX_INCLUDE_THREAD_THREAD_H_

#include <thread>
#include <cassert>
#include <functional>

#include "lazybox/toy/NonCopyAble.hpp"
#include "lazybox/toy/DEFER.hpp"

namespace lbox {

class Thread {
public:
    Thread() = default;
    virtual ~Thread() {
        if(work_thread_) {
            delete work_thread_;
            work_thread_ = nullptr;
        }
    }

    NON_COPYABLE_(Thread);

    bool Start() {
        work_thread_ = new std::thread(std::bind(&Thread::WorkingThread, this));
        assert(work_thread_ && "thread is nullptr, use Start first");
        return !!work_thread_;
    }
    void Stop() {
        assert(work_thread_ && "thread is nullptr, use Start first");
        active_ = false;
    }
    bool JoinAble() {
        assert(work_thread_ && "thread is nullptr, use Start first");
        return work_thread_->joinable();
    }
    void Join() {
        if(JoinAble())
            work_thread_->join();
    }
    void Detach() {
        assert(work_thread_ && "thread is nullptr, use Start first");
        work_thread_->detach();
    }

    std::thread::id ThreadID() { return work_thread_->get_id(); }
    bool            Active() { return active_; }

    virtual bool OnThreadStart() { return true; }
    virtual void OnThreadOver() {}

    virtual bool Process() = 0;

private:
    void WorkingThread();

private:
    volatile bool active_{false};
    std::thread  *work_thread_{nullptr};
};

inline void Thread::WorkingThread() {
    active_ = true;
    DEFER([&]() { active_ = false; });

    if(!this->OnThreadStart()) {
        return;
    }

    while(active_) {
        active_ &= this->Process();
    }

    this->OnThreadOver();
}

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_THREAD_H_
