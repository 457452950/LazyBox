#include <cassert>
#include "thread/Lock.h"
#include "thread/ThreadPool.h"
#include "thread/MessageQueue.h"

int        cur = 1;
static int sum = 0;

std::mutex m;
// lbox::FastLock m;

void producerThread(lbox::MQueue<int> &queue) {
    //    lbox::UniqueLock mm(m);
    std::unique_lock mm(m);
    queue.Push(cur);
    ++cur;
}

void consumerThread(lbox::MQueue<int> &queue, int answer) {
    while(true) {
        if(queue.Empty())
            continue;

        //    lbox::UniqueLock mm(m);
        std::unique_lock mm(m);
        sum += queue.Get();

        if(answer == sum) {
            return;
        }
    }
}

int main() {
    using namespace std::chrono_literals;

    lbox::ThreadPool  pool;
    lbox::MQueue<int> queue;

    const int   times = 30000;
    std::thread consumer_thr{&consumerThread, std::ref(queue), times * (times + 1) / 2};

    for(int i = 1; i <= times; ++i) {
        pool.Push(std::bind(&producerThread, std::ref(queue)));
    }


    //    std::this_thread::sleep_for(200ms);
    consumer_thr.join();

    printf("sum %d", sum);
    assert(sum == times * (times + 1) / 2);


    pool.Quit();
    pool.Join();

    return 0;
}
