#include <cassert>
#include "thread/ThreadPool.hpp"
#include "thread/Channel.hpp"
#include "Chrono.h"

#include "Toy/Instance.hpp"
#include "Toy/DEFER.hpp"

int64_t        cur = 1;
static int64_t sum = 0;

// std::mutex m;
lbox::FastLock m;

void producerThread(lbox::Channel<int64_t> &queue) {
    std::lock_guard mm{m};
    //    std::unique_lock mm(m);
    queue.Push(cur);
    ++cur;
    //    if(cur % 100000) {
    //
    //    } else {
    //        printf("%lld\n", cur);
    //    }
}

void consumerThread(lbox::Channel<int64_t> &queue, int64_t answer) {
    while(true) {
        if(queue.Empty())
            continue;

        std::lock_guard mm{m};
        //        std::unique_lock mm(m);
        sum += queue.Get();

        if(answer == sum) {
            return;
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
using T_p = lbox::Instance<TTT>;

int main() {
    T_p::Init(1, 'a');
    DEFER([]() { T_p::Destroy(); });

    using namespace std::chrono_literals;

    Pool                   pool;
    lbox::Channel<int64_t> queue;

    pool.Start();
    auto cl = lbox::GetTickMs64();

    const int64_t times  = 120000;
    auto          answer = times * (times + 1) / 2;
    std::thread   consumer_thr{&consumerThread, std::ref(queue), answer};

    for(int64_t i = 1; i <= times; ++i) {
        pool.Push(std::bind(&producerThread, std::ref(queue)));
    }

    printf("%lld \n", lbox::GetTickMs64() - cl);

    //    std::this_thread::sleep_for(200ms);
    consumer_thr.join();

    printf("sum %lld  %lld\n", sum, answer);
    assert(sum == answer);

    printf("%lld \n", lbox::GetTickMs64() - cl);

    pool.Quit();
    pool.Join();

    return 0;
}
