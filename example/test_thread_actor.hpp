#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_ACTOR_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_ACTOR_HPP_

#include "lazybox/thread/ThreadPool.hpp"
#include "lazybox/thread/Actor.hpp"
#include "lazybox/test/Test.h"

namespace {
const int        actor_times  = 1000;
int              actor_answer = 0;
std::atomic_bool actor_active = true;
} // namespace

TCASE(Thread, Actor) {
    class RecvActor : public lbox::Actor<int> {
    public:
        RecvActor() = default;

        void operator()() {
            while(actor_active) {
                int i = 0;
                if(this->Get(i))
                    actor_answer += i;
                else
                    std::this_thread::yield();
            }
        }
    };

    RecvActor        actor;
    lbox::ThreadPool pool;
    pool.Start(6);

    auto fut = pool.Submit(std::packaged_task<void()>(std::bind(&RecvActor::operator(), &actor)));

    auto func  = [&actor]() { actor.Send(1); };
    auto func2 = [&pool, &func]() {
        for(int i = 0; i < actor_times; ++i) {
            pool.Submit(func);
        }
    };

    for(int i = 0; i < actor_times; ++i) {
        pool.Submit(func2);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    actor_active = false;

    fut.get();
    pool.Quit();
    pool.Join();

    printf("answer %d\n", actor_answer);
    TASSERT(actor_answer == actor_times * actor_times, "answer should be times^2");
}

namespace {
int32_t          wactor_answer = 0;
std::atomic_bool wactor_active = true;
} // namespace
TCASE(Thread, WaitableActor) {
    class RecvActor : public lbox::WaitableActor<int> {
    public:
        RecvActor() = default;

        void operator()() {
            while(wactor_active) {
                int i = 0;
                if(this->Wait2Get(i)) {
                    if(i == 0) {
                        return;
                    }
                    wactor_answer++;
                } else {
                    std::this_thread::yield();
                }
            }
        }

        void operator()(int i) {
            while(wactor_active) {
                int ii = 0;
                if(this->Wait2Get(ii, std::chrono::milliseconds(i)))
                    wactor_answer += ii;
                else
                    std::this_thread::yield();
            }
        }
    };

    RecvActor        actor;
    lbox::ThreadPool pool;
    pool.Start(8);

    //    auto fut = pool.Submit(std::packaged_task<void()>([&actor]() { actor(); }));
    auto fut = pool.Submit(std::packaged_task<void()>([&actor]() { actor(2); }));

    auto func  = [&actor]() { actor.Send(1); };
    auto func2 = [&pool, &func]() {
        for(int i = 0; i < actor_times; ++i) {
            pool.Submit(func);
        }
    };

    for(int i = 0; i < actor_times; ++i) {
        pool.Submit(func2);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    wactor_active = false;
    actor.Send(0);

    fut.get();
    printf("waitable answer %d\n", wactor_answer);
    pool.Quit();
    pool.Join();

    TASSERT(wactor_answer == actor_times * actor_times, "answer should be times^2");
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_ACTOR_HPP_
