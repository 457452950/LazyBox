#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_CHANNEL_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_CHANNEL_HPP_

#include "lazybox/thread/Channel.hpp"
#include "lazybox/test/Test.h"
#include "lazybox/thread/ThreadPool.hpp"

TCASE(Thread, Channel) {
    lbox::Channel<int> channel;

    std::atomic_bool flag{true};

    std::atomic_char32_t send_times = 0;
    std::atomic_char32_t recv_times = 0;

    auto prod = [&]() {
        while(flag) {
            channel.Push(1);
            send_times++;
            std::this_thread::yield();
        }
    };

    auto cons = [&]() {
        int value;
        while(flag) {
            if(channel.Get(value)) {
                recv_times++;
            } else {
                std::this_thread::yield();
            }
        }
    };

    lbox::ThreadPool pool;
    pool.Start();

    pool.Submit(prod);
    pool.Submit(prod);

    pool.Submit(cons);
    pool.Submit(cons);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    flag = false;
    pool.Quit();
    pool.Join();

    printf("send_times: %d\n", send_times.load());
    printf("recv_times: %d\n", recv_times.load());
}


TCASE(Thread, WaitableChannel) {
    {
        // case 1:  push block
        auto res = std::async([]() {
            lbox::WaitableChannel<int, 5> channel;

            auto r = std::async(std::launch::async, [&]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                int i;
                channel.Get(i);
            });

            for(int i = 0; i < 6; i++) {
                channel.Push(1);
            }

            r.get();
            channel.Close();
            channel.Clear();
        });

        TASSERT(res.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout, "push has not block");
        res.get();
    }
    {
        // case 2:  get block
        auto res = std::async([]() {
            lbox::WaitableChannel<int, 5> channel;

            auto r = std::async(std::launch::async, [&]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                channel.Push(1);
            });

            int i;
            channel.Get(i);

            r.get();
            channel.Close();
            channel.Clear();
        });

        TASSERT(res.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout, "get has not block");
        res.get();
    }
    {
        lbox::WaitableChannel<int, 5> channel;
        TASSERT(channel.Size() == 0, "channel size is not 0");
        TASSERT(channel.Empty(), "channel is not empty");


        channel.Push(1);
        TASSERT(channel.Size() == 1, "channel size is not 1");

        channel.Push(2);
        TASSERT(channel.Size() == 2, "channel size is not 2");

        channel.Push(3);
        TASSERT(channel.Size() == 3, "channel size is not 3");

        channel.Push(4);
        TASSERT(channel.Size() == 4, "channel size is not 4");

        channel.Push(5);
        TASSERT(channel.Size() == 5, "channel size is not 5");

        int i;

        channel.Get(i);
        TASSERT(i == 1, "i is not 1");
        TASSERT(channel.Size() == 4, "channel size is not 4");

        channel.Get(i);
        TASSERT(i == 2, "i is not 2");
        TASSERT(channel.Size() == 3, "channel size is not 3");

        channel.Get(i);
        TASSERT(i == 3, "i is not 3");
        TASSERT(channel.Size() == 2, "channel size is not 2");

        channel.Get(i);
        TASSERT(i == 4, "i is not 4");
        TASSERT(channel.Size() == 1, "channel size is not 1");

        channel.Get(i);
        TASSERT(i == 5, "i is not 5");
        TASSERT(channel.Size() == 0, "channel size is not 0");

        TASSERT(channel.Size() == 0, "channel size is not 0");
        TASSERT(channel.Empty(), "channel is not empty");
    }
}


#endif // LAZYBOX_EXAMPLE_TEST_THREAD_CHANNEL_HPP_
