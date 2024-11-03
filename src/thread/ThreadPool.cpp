#include "lazybox/thread/ThreadPool.hpp"
#include "lazybox/Assert.hpp"

namespace lbox {

ThreadPool::ThreadPool() = default;

ThreadPool::~ThreadPool() = default;

void ThreadPool::Start(std::size_t count) {
    this->thread_active_count_ = count;
    {
        std::lock_guard<std::mutex> lock{this->control_mutex_};
        for(int i = 0; i < count; ++i) {
            newThread();
        }
    }
}
void ThreadPool::Join() {
    this->WakeUpAll();
    for(auto &it : this->workers_) {
        it->Join();
    }
}
void ThreadPool::Detach() {
    this->WakeUpAll();
    for(auto &it : this->workers_) {
        it->Detach();
    }
}
void ThreadPool::Quit() { this->active_.store(false); }
void ThreadPool::checkThreadCount() {
    auto count = SetThreadCount(this->task_que_.Size());

    if(this->thread_active_count_ == count && this->workers_.size() >= count) {
        //
        return;
    }
    std::cout << "count " << count << " " << this->thread_active_count_ << " " << this->workers_.size() << std::endl;

    //
    if(this->thread_active_count_ < count) {
        //
        if(this->workers_.size() < count) {
            auto add = count - this->workers_.size();
            assert(add > 0);

            for(int i = 0; i < add; ++i) {
                this->newThread();
            }
            Assert(count == this->workers_.size(), "workers count not equal {} != {}", count, this->workers_.size());
        }
    }

    thread_active_count_ = count;
}

ThreadPool::Worker::Worker(ThreadPool *pool, int index) : thread_index_(index), pool_(pool) {
    assert(pool_);
    this->worker_ = std::make_unique<std::thread>(&ThreadPool::Worker::thread_work_handle, this);
}
ThreadPool::Worker::~Worker() {
    if(pool_) {
        pool_ = nullptr;
    }
}

void ThreadPool::Worker::thread_work_handle() {
    while(pool_->active_.load(std::memory_order_relaxed)) {
        std::unique_lock<std::mutex> uni(pool_->control_mutex_);

        while(pool_->active_.load(std::memory_order_relaxed) && pool_->task_que_.Empty()) {
            pool_->control_ca_.wait(uni, [this]() -> bool {
                // non-block when not active
                if(!pool_->active_.load(std::memory_order_relaxed)) {
                    return true;
                }

                pool_->checkThreadCount();

                // block when thread_index_ >= thread_active_count_
                if(this->thread_index_ >= pool_->thread_active_count_) {
                    return false;
                }

                // block when task queue is empty
                if(this->pool_->task_que_.Empty()) {
                    return false;
                } else {
                    return true;
                }
            });
        }

        if(!pool_->active_.load(std::memory_order_relaxed)) {
            break;
        }

        Task task;
        pool_->task_que_.Pop(task);

        uni.unlock();

        task();
    }
    // thread_end
    //    std::cout << "thread end " << this->thread_index_ << std::endl;
    this->pool_->WakeUp();
}
void ThreadPool::Worker::Join() {
    if(this->worker_) {
        if(this->worker_->joinable()) {
            this->worker_->join();
        }
    }
}
void ThreadPool::Worker::Detach() {
    if(this->worker_) {
        if(this->worker_->joinable()) {
            this->worker_->detach();
        }
    }
}

} // namespace lbox
