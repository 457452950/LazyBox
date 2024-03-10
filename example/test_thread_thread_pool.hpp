#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_THREAD_POOL_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_THREAD_POOL_HPP_


#include "lazybox/thread/ThreadPool.hpp"
#include "lazybox/test/Test.h"

TCASE(Thread, ThreadPool1) {
    lbox::ThreadPool pool;

    pool.Start();
    std::atomic_int64_t counter(0);

    const auto times = 10000;

    for(int i = 0; i < times; ++i) {
        pool.Submit([&counter]() { counter.fetch_add(1, std::memory_order_relaxed); });
    }
    while(counter.load() < times) {
        std::this_thread::yield();
    }

    pool.Quit();
    pool.Join();

    TASSERT(counter.load() == times, "counter is not 10000");
}

TCASE(Thread, ThreadPool2) {
    lbox::ThreadPool pool1;
    lbox::ThreadPool pool2;

    pool1.Start(6);
    pool2.Start(6);

    std::atomic_int64_t counter(0);
    int                 count{0};

    const auto times = 1000;

    auto func1 = [&counter, &count]() {
        ++count;
        counter.fetch_add(1, std::memory_order_relaxed);
    };
    auto func2 = [&]() {
        for(int i = 0; i < times; ++i) {
            pool2.Submit(func1);
        }
    };


    for(int i = 0; i < times; ++i) {
        pool1.Submit(func2);
    }

    while(counter.load() < times * times) {
        std::this_thread::yield();
    }

    pool1.Quit();
    pool2.Quit();
    pool1.Join();
    pool2.Join();

    printf("counter is %ld\n", counter.load());
    TASSERT(counter.load() == times * times, "counter is not 10000");
    printf("count is %d\n", count);
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_THREAD_POOL_HPP_
