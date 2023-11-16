#include <cassert>
#include "thread/Lock.h"
#include "thread/ThreadPool.h"

static int sum = 0;

// std::mutex m;
lbox::FastLock m;

void count(int i) {
    //    std::unique_lock mm(m);
    lbox::UniqueLock mm(m);
    sum += i;
}

int main() {
    using namespace std::chrono_literals;

    lbox::ThreadPool pool;

    const int times = 100;
    //    pool.Push(std::bind(&count, 1));
    for(int i = 1; i <= times; ++i) {
        pool.Push(std::bind(&count, i));
    }

    std::this_thread::sleep_for(200ms);

    assert(sum == times * (times + 1) / 2);
    printf("sum %d", sum);
    pool.Quit();
    pool.Join();

    return 0;
}
