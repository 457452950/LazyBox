#include <cassert>
#include <iostream>
#include "lazybox/thread/ThreadPool.hpp"
#include "lazybox/thread/Channel.hpp"
#include "lazybox/thread/Actor.hpp"
#include "lazybox/Chrono.h"

#include "lazybox/Toy/Instance.hpp"
#include "lazybox/Toy/DEFER.hpp"


int64_t        cur = 1;
static int64_t sum = 0;

// std::mutex m;
lbox::FastLock m;

void producerThread(lbox::Channel<int64_t> &queue) {
    std::lock_guard mm{m};
    queue.Push(cur);
    ++cur;
    if(cur % 1000) {

    } else {
        printf("push %lld\n", cur);
    }
}

void consumerThread(lbox::Channel<int64_t> &queue, int64_t answer) {
    while(true) {
        if(queue.Empty())
            continue;

        std::lock_guard mm{m};
        int64_t         v;
        if(!queue.Get(v)) {
            continue;
        }
        sum += v;

        if(answer == sum) {
            return;
        } else {
            //            std::cout << "sum: " << sum << " v " << v << std::endl;
        }
    }
}

class Pool : public lbox::ThreadPool {
public:
    std::size_t AllocateThread(std::size_t task_count) override {
        return 6;
        auto c = std::max<std::size_t>(4, std::min<std::size_t>(40, task_count / 2));
        //        printf("%zu\n", task_count);
        return c;
    }
};

class TTT {
public:
    TTT(int a, char b) {}
};
using T_p = lbox::GlobalInstance<TTT>;
class TTTT {
public:
    TTTT() {}
};
using TT_p = lbox::Instance<TTTT>;

class Consumer : public lbox::Actor<TTTT> {
    bool SendingMessageFilter(const TTTT &v) override {
        std::cout << "a " << std::endl;
        return true;
    }
};

void vvoid(void) { std::cout << "vvoid " << lbox::GetTickMs64() << std::endl; }

int main() {
    Consumer c;
    TTTT     a;
    c.Send(TTTT{});
    c.Send(a);
    c.Send(std::move(a));

    T_p::Init(1, 'a');
    DEFER([]() { T_p::Destroy(); });

    TT_p::GetInstance();

    using namespace std::chrono_literals;

    Pool                   pool;
    lbox::Channel<int64_t> queue;

    pool.Start();
    auto cl = lbox::GetTickMs64();

    const int64_t times  = 10000;
    auto          answer = times * (times + 1) / 2;
    std::thread   consumer_thr{&consumerThread, std::ref(queue), answer};


    pool.Submit([]() { std::cout << "lam vvoid " << lbox::GetTickMs64() << std::endl; });
    pool.Submit([]() {
        std::cout << "lam int " << lbox::GetTickMs64() << std::endl;
        return 1;
    });
    pool.Submit(&vvoid);
    pool.Submit(std::bind(vvoid));
    pool.Submit(std::bind(
            [](int i, int v) -> int {
                std::cout << "bind lam int " << lbox::GetTickMs64() << std::endl;
                return 2;
            },
            1,
            2));

    for(int i = 0; i < 1000; ++i) {
        std::async(std::launch::async, [&pool]() { pool.Submit([]() {}); });
    }
    //
    //    auto res1 =
    //            pool.Submit(std::packaged_task([]() { std::cout << "lam vvoid " << lbox::GetTickMs64() << std::endl;
    //            }));
    //    auto res2 = pool.Submit(std::packaged_task([]() -> int {
    //        std::cout << "lam int " << lbox::GetTickMs64() << std::endl;
    //        return 1;
    //    }));
    //    auto res3 = pool.Submit(std::packaged_task(&vvoid));
    //    auto res5 = pool.Submit(std::packaged_task<void()>(std::bind(vvoid)));
    //    auto res6 = pool.Submit(std::packaged_task<int()>(std::bind([](int i, int v) -> int { return 2; }, 1, 2)));
    auto res7 = pool.Submit(std::packaged_task([]() {
        std::this_thread::sleep_for(2000ms);
        return 164;
    }));
    auto res8 = pool.Submit(std::packaged_task([]() {
        std::this_thread::sleep_for(5000ms);
        return 64;
    }));

    for(int64_t i = 1; i <= times; ++i) {
        pool.Submit(std::bind(&producerThread, std::ref(queue)));
    }

    printf("after join %lld \n", lbox::GetTickMs64() - cl);

    //    std::this_thread::sleep_for(200ms);
    consumer_thr.join();

    printf("sum %lld answer %lld\n", sum, answer);
    assert(sum == answer);

    printf("%lld \n", lbox::GetTickMs64() - cl);

    std::cout << "res7 " << res7.get() << " " << lbox::GetTickMs64() - cl << std::endl;

    pool.Quit();
    pool.Join();
    printf("join \n");

    std::cout << "res8 " << res8.get() << " " << lbox::GetTickMs64() - cl << std::endl;

    return 0;
}
