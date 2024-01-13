#include "thread/ThreadPool.hpp"
#include <cassert>

namespace lbox {

ThreadPool::ThreadPool() = default;

ThreadPool::~ThreadPool() {
    for(auto it : workers_) {
        it->Join();
    }

    this->workers_.clear();
    this->thread_active_count_ = 0;
}
void ThreadPool::Start(std::size_t count) {
    this->thread_active_count_ = count;
    for(int i = 0; i < thread_active_count_; ++i) {
        this->newThread();
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
void ThreadPool::Quit() {
    for(auto &thread : workers_) {
        thread->Quit();
    }
}
void ThreadPool::checkThreadCount() {
    auto count = AllocateThread(this->task_que_.size());

    if(this->thread_active_count_ == count) {
        //
        return;
    } else if(this->thread_active_count_ < count) {
        //
        if(this->workers_.size() < count) {
            auto add = count - this->workers_.size();
            assert(add > 0);

            for(int i = 0; i < add; ++i) {
                this->newThread();
            }
            assert(count == this->workers_.size());
        }
    }

    thread_active_count_ = count;
}

ThreadPool::Worker::Worker(ThreadPool *pool, int index) : thread_index_(index), pool_(pool) {
    assert(pool_);
    this->worker_ = new std::thread(&ThreadPool::Worker::thread_work_handle, this);
}
ThreadPool::Worker::~Worker() {
    if(this->worker_) {
        delete this->worker_;
        worker_ = nullptr;
    }
    if(pool_) {
        pool_ = nullptr;
    }
}

void ThreadPool::Worker::thread_work_handle() {
    // thread_start
    assert(this);

    while(active_) {
        //////////
        assert(this);
        assert(pool_);
        std::unique_lock uni(pool_->control_mutex_);

        while(pool_->task_que_.empty()) {
            if(!active_) {
                break;
            }

            pool_->control_ca_.wait(uni, [this]() {
                if(!this->active_) {
                    return true;
                }

                return !this->pool_->task_que_.empty() && (this->thread_index_ < pool_->thread_active_count_);
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
