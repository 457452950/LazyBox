#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_THREADPOOL_H_
#define LAZYBOX_INCLUDE_THREAD_THREADPOOL_H_

#include <vector>
#include <mutex>
#include <thread>
#include <future>
#include <queue>
#include <functional>

namespace lbox {

static constexpr unsigned int default_thread_count = 4;

class ThreadPool {
    using task_type = std::function<void()>;
    struct Task {
        task_type task;
    };

    class Worker {
    public:
        explicit Worker(ThreadPool *pool);
        virtual ~Worker();

        Worker(const Worker &)                = delete;
        Worker &operator=(const Worker &)     = delete;
        Worker(Worker &&other) noexcept       = delete;
        Worker &operator=(Worker &&) noexcept = delete;

        void Join();
        void Detach();
        void Quit();

    private:
        void thread_work_handle();

    private:
        std::thread     *worker_{nullptr};
        std::atomic_bool active_{true};
        ThreadPool      *pool_{nullptr};
    };

public:
    explicit ThreadPool(std::size_t thread_count = default_thread_count);
    virtual ~ThreadPool() = default;

    void Start(std::size_t count);
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

    std::size_t GetThreadCount() const { return thread_count_; }

protected:
    // return total thread count
    virtual std::size_t AllocateThread([[maybe_unused]] std::size_t task_count) { return default_thread_count; };

    void WakeUpAll() { this->control_ca_.notify_all(); }
    void WakeUp() { this->control_ca_.notify_one(); }

private:
    void checkThreadCount();

    void newThread() { this->workers_.push_back(new Worker(this)); }

private:
    std::size_t           thread_count_{0};
    std::vector<Worker *> workers_;
    std::queue<Task>      task_que_;

    // mutex
    std::mutex              control_mutex_;
    std::condition_variable control_ca_;
};

} // namespace lbox
#endif // LAZYBOX_INCLUDE_THREAD_THREADPOOL_H_
