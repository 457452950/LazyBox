#include "thread/ThreadPool.h"
#include <cassert>

namespace lbox {

ThreadPool::ThreadPool(std::size_t thread_count) : thread_count_(thread_count) {
    for(int i = 0; i < thread_count_; ++i) {
        this->newThread();
    }
}
void ThreadPool::Start(std::size_t count) {
    this->thread_count_ = count;
    for(int i = 0; i < thread_count_; ++i) {
        this->newThread();
    }
}
void ThreadPool::Join() {
    for(auto &thread : workers_) {
        thread->Join();
    }
    this->WakeUpAll();
}
void ThreadPool::Detach() {
    for(auto &thread : workers_) {
        thread->Detach();
    }
    this->WakeUpAll();
}
void ThreadPool::Quit() {
    for(auto &thread : workers_) {
        thread->Quit();
    }
    this->WakeUpAll();
}
void ThreadPool::checkThreadCount() {
    auto count = AllocateThread(this->task_que_.size());

    if(this->thread_count_ == count) {
        //
        return;
    } else if(this->thread_count_ < count) {
        //
        auto add = count - this->thread_count_;
        assert(add > 0);

        for(int i = 0; i < add; ++i) {
            this->newThread();
        }
    } else {
        //
        auto less = this->thread_count_ - count;
        assert(less < this->thread_count_);

        for(int i = 1; i <= less; ++i) {
            this->workers_[thread_count_ - i]->Quit();
        }
    }
}

ThreadPool::Worker::Worker(ThreadPool *pool) : pool_(pool) {
    assert(pool_);
    this->worker_ = new std::thread(&ThreadPool::Worker::thread_work_handle, this);
}
ThreadPool::Worker::~Worker() {
    if(this->worker_) {
        if(worker_->joinable()) {
            worker_->join();
        }
        delete worker_;
        worker_ = nullptr;
    }
}

void ThreadPool::Worker::thread_work_handle() {
    // thread_start
    assert(this);

    while(active_) {
        //////////
        std::unique_lock uni(pool_->control_mutex_);

        while(pool_->task_que_.empty()) {
            if(!active_) {
                break;
            }

            pool_->control_ca_.wait(uni, [this]() {
                if(!this->active_) {
                    return true;
                }

                return !this->pool_->task_que_.empty();
            });
        }

        if(!active_) {
            break;
        }

        Task task = pool_->task_que_.front();
        pool_->task_que_.pop();

        uni.unlock();
        ///////////

        if(task.task)
            task.task();
    }

    // thread_end
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
void ThreadPool::Worker::Quit() { this->active_.store(false); }

} // namespace lbox
  // namespace lbox