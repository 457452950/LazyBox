#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_THREADPOOL_HPP_
#define LAZYBOX_INCLUDE_THREAD_THREADPOOL_HPP_

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <future>
#include <condition_variable>
#include <type_traits>
#include <iostream>

#include "Lock.hpp"
#include "lazybox/base/TypeTraits.hpp"
#include "lazybox/Toy/NonCopyAble.hpp"
#include "lazybox/thread_safe/Queue.hpp"

namespace lbox {

class ThreadPool : public NonCopyAble {
public:
    /* static */
    static auto DefaultThreadCount() {
        auto tc = std::thread::hardware_concurrency();
        return (tc < 2) ? 1 : (tc - 1);
    }

    class function_wrapper : public NonCopyAble {
        struct impl_base {
            virtual void call()  = 0;
            virtual ~impl_base() = default;
        };

        template <typename F>
        struct impl_type : impl_base {
            using Func = typename std::remove_cv<F>::type;
            Func f;
            impl_type(Func &&f_) : f(std::forward<Func>(f_)) {}
            void call() { f(); }
        };

        std::unique_ptr<impl_base> impl;

    public:
        function_wrapper() = default;

        template <typename F>
        function_wrapper(F &&f) : impl(std::make_unique<impl_type<F>>(std::forward<F>(f))) {}

        function_wrapper(function_wrapper &&other) : impl(std::move(other.impl)) {}
        function_wrapper &operator=(function_wrapper &&other) {
            impl = std::move(other.impl);
            return *this;
        }

        void operator()() { impl->call(); }
    };


private:
    using Task = function_wrapper;

    class Worker : public NonCopyAble {
    public:
        explicit Worker(ThreadPool *pool, int index);
        virtual ~Worker();

        void Join();
        void Detach();

    private:
        void thread_work_handle();

    private:
        int                          thread_index_{-1};
        std::unique_ptr<std::thread> worker_{nullptr};
        ThreadPool                  *pool_{nullptr};
    };

public:
    ThreadPool();
    virtual ~ThreadPool();

    void Start(std::size_t count = 4);
    void Join();
    void Detach();
    void Quit();

    // for lambda, bind, function
    template <class F>
    void Submit(F &&f) {
        Task task{std::forward<F>(f)};

        this->task_que_.Push(std::move(task));
        this->WakeUpAll();
    }

    template <class ResType>
    auto Submit(std::packaged_task<ResType(void)> &&f) -> std::future<ResType> {
        auto res = f.get_future();
        Task task{std::move(f)};

        this->task_que_.Push(std::move(task));

        this->WakeUpAll();
        return res;
    }


    std::size_t GetThreadCount() {
        std::lock_guard<std::mutex> lock{this->control_mutex_};
        return thread_active_count_;
    }

protected:
    // return total thread count, lock has been acquired
    virtual std::size_t SetThreadCount([[maybe_unused]] std::size_t task_count) { return thread_active_count_; };


    void WakeUpAll() {
        std::atomic_thread_fence(std::memory_order_acquire);
        this->control_ca_.notify_all();
        std::atomic_thread_fence(std::memory_order_release);
    }
    void WakeUp() {
        std::atomic_thread_fence(std::memory_order_acquire);
        this->control_ca_.notify_one();
        std::atomic_thread_fence(std::memory_order_release);
    }

private:
    void checkThreadCount();

    void newThread() {
        //        std::cout << "new thread, create thread id : " << std::this_thread::get_id() << std::endl;
        this->workers_.push_back(std::make_unique<Worker>(this, static_cast<int>(this->workers_.size())));
    }

private:
    unsigned int                         thread_active_count_{4};
    std::atomic_bool                     active_{true};
    std::vector<std::unique_ptr<Worker>> workers_;
    thread_safe::Queue<Task>             task_que_;

    // mutex
    std::mutex              control_mutex_;
    std::condition_variable control_ca_;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_THREADPOOL_HPP_
