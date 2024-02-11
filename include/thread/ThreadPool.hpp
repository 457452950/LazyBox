#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_THREADPOOL_HPP_
#define LAZYBOX_INCLUDE_THREAD_THREADPOOL_HPP_

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>

#include "Lock.hpp"
#include "Toy/NonCopyAble.hpp"

namespace lbox {

static constexpr unsigned int default_thread_count = 4;

class ThreadPool : public NonCopyAble {
    using task_type = std::function<void()>;
    struct Task {
        task_type task;
    };

    class Worker : public NonCopyAble {
    public:
        explicit Worker(ThreadPool *pool, int index);
        virtual ~Worker();

        void Join();
        void Detach();
        void Quit();

    private:
        void thread_work_handle();

    private:
        int              thread_index_{-1};
        std::thread     *worker_{nullptr};
        std::atomic_bool active_{true};
        ThreadPool      *pool_{nullptr};
    };

public:
    ThreadPool();
    virtual ~ThreadPool();

    void Start(std::size_t count = default_thread_count);
    void Join();
    void Detach();
    void Quit();

    template <class T>
    void Push(T &&task) {
        {
            std::unique_lock uni(this->control_mutex_);
            this->task_que_.emplace(std::forward<task_type>(task));
            this->checkThreadCount();
        }
        this->WakeUpAll();
    }

    std::size_t GetThreadCount() const { return thread_active_count_; }

protected:
    // return total thread count
    virtual std::size_t AllocateThread([[maybe_unused]] std::size_t task_count) { return default_thread_count; };

    void newThread() { this->workers_.push_back(new Worker(this, static_cast<int>(this->workers_.size()))); }

    void WakeUpAll() { this->control_ca_.notify_all(); }
    void WakeUp() { this->control_ca_.notify_one(); }

private:
    void checkThreadCount();

private:
    std::size_t           thread_active_count_{0};
    std::vector<Worker *> workers_;
    std::queue<Task>      task_que_;

    // mutex
    std::mutex              control_mutex_;
    std::condition_variable control_ca_;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_THREADPOOL_HPP_
