#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_INSTANCE_HPP_
#define LAZYBOX_EXAMPLE_TEST_INSTANCE_HPP_

#include <thread>
#include <future>
#include <iostream>

#include "lazybox/test/Test.h"
#include "lazybox/toy/Instance.hpp"

class A {};

TCASE(Instance, ThreadLocal) {
    using AA = lbox::ThreadInstance<A>;

    auto func = []() {
        //        std::cout << "thread id" << std::this_thread::get_id() << std::endl;
        return AA::Init();
    };

    auto addr1 = func();
    auto addr2 = std::async(std::launch::deferred, func).get();
    TASSERT(addr1 == addr2, "Addresses should be the same");


    auto addr3 = std::async(std::launch::async, func).get();
    TASSERT(addr1 != addr3, "Addresses should not be the same");

    AA::Destroy();
    delete addr3;
}

TCASE(Instance, Global) {
    using AA = lbox::GlobalInstance<A>;
    AA::Init();

    TASSERT(AA::GetInstance() == AA::GetInstance(), "Addresses should be the same");
    AA::Destroy();
}

TCASE(Instance, Instance) {
    using AA = lbox::Instance<A>;

    std::atomic_bool flag{false};
    auto             func = [&flag]() {
        //        std::cout << "thread id" << std::this_thread::get_id() << std::endl;
        while(!flag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        return AA::GetInstance();
    };

    const auto                    test_thread_count = 10;
    std::vector<A *>              answer;
    std::vector<std::thread>      threads;
    std::vector<std::future<A *>> futures;

    for(int i = 0; i < test_thread_count; ++i) {
        futures.push_back(std::async(func));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    flag.store(true);

    for(int i = 0; i < test_thread_count; ++i) {
        answer.push_back(futures[i].get());
    }

    std::for_each(answer.begin(), answer.end(), [&](A *a) { TASSERT(answer[0] == a, "Addresses should be the same"); });
    AA::Destroy();
}

#endif // LAZYBOX_EXAMPLE_TEST_INSTANCE_HPP_
