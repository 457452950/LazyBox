#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_QUEUE_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_QUEUE_HPP_

#include <queue>

#include "../thread/Lock.hpp"

namespace lbox::thread_safe {

template <class T>
class Queue : public NonCopyAble {
public:
    Queue()  = default;
    ~Queue() = default;

    template <class U>
    void Push(U &&value) {
        std::lock_guard<FastLock> lock(lock_);
        queue_.push(std::forward<U>(value));
    }

    [[nodiscard]] bool Pop(T &value) {
        std::lock_guard<FastLock> lock(lock_);
        if(queue_.empty()) {
            return false;
        }
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    size_t Size() {
        std::lock_guard<FastLock> lock(lock_);
        return queue_.size();
    }

    [[nodiscard]] bool Empty() {
        std::lock_guard<FastLock> lock(lock_);
        return queue_.empty();
    }

    template <typename... _Args>
    void Emplace(_Args &&...__args) {
        std::lock_guard<FastLock> lock(lock_);
        queue_.emplace(std::forward<_Args>(__args)...);
    }

    std::queue<T> Clone() {
        std::lock_guard<FastLock> lock(lock_);

        std::queue<T> q = this->queue_;
        return q;
    }

    void Clear() {
        std::lock_guard<FastLock> lock(lock_);
        this->queue_.swap(std::queue<T>());
    }

private:
    std::queue<T> queue_;
    FastLock      lock_;
};

} // namespace lbox::thread_safe

#endif // LAZYBOX_INCLUDE_LAZYBOX_THREAD_SAFE_QUEUE_HPP_
