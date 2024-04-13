#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_THREAD_THREAD_SAFE_QUEUE_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_THREAD_THREAD_SAFE_QUEUE_HPP_

#include <queue>

#include "lazybox/thread/Lock.hpp"

namespace lbox::thread_safe {

template <class T>
class Queue {
    using Data = T;

public:
    Queue()  = default;
    ~Queue() = default;

    NON_COPYABLE_(Queue);

    template <class U>
    void Push(U &&value) {
        std::lock_guard<FastLock> lock(lock_);
        queue_.push(std::forward<U>(value));
    }

    [[nodiscard]] bool Pop(Data &value) {
        std::lock_guard<FastLock> lock(lock_);

        if(queue_.empty()) {
            return false;
        }

        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    Data Pop() noexcept(false) {
        std::lock_guard<FastLock> lock(lock_);
        if(this->queue_.empty()) {
            throw std::out_of_range("thread_safe::Queue::Pop(), queue empty");
        }

        Data &&v = std::move(this->queue_.front());
        queue_.pop();
        return std::move(v);
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

    std::queue<Data> Dump() {
        std::lock_guard<FastLock> lock(lock_);

        std::queue<Data> q = this->queue_;
        return q;
    }

    void Clear() {
        std::lock_guard<FastLock> lock(lock_);
        std::queue<Data>          t;
        this->queue_.swap(t);
    }

private:
    std::queue<Data> queue_;
    FastLock         lock_;
};


template <class T>
class Queue<T *> {
    using Data = T *;

public:
    Queue()  = default;
    ~Queue() = default;

    NON_COPYABLE_(Queue);

    template <class U>
    void Push(U &&value) {
        std::lock_guard<FastLock> lock(lock_);
        queue_.push(std::forward<U>(value));
    }

    [[nodiscard]] bool Pop(Data &value) {
        std::lock_guard<FastLock> lock(lock_);

        if(queue_.empty()) {
            return false;
        }

        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    Data Pop() {
        std::lock_guard<FastLock> lock(lock_);
        if(this->queue_.empty()) {
            return nullptr;
        }

        Data &&v = std::move(this->queue_.front());
        queue_.pop();
        return v;
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

    std::queue<Data> Dump() {
        std::lock_guard<FastLock> lock(lock_);

        std::queue<Data> q = this->queue_;
        return q;
    }

    void Clear() {
        std::lock_guard<FastLock> lock(lock_);
        std::queue<Data>          t;
        this->queue_.swap(t);
    }

private:
    std::queue<Data> queue_;
    FastLock         lock_;
};


template <class T>
class Queue<std::shared_ptr<T>> {
    using Data = std::shared_ptr<T>;

public:
    Queue()  = default;
    ~Queue() = default;

    NON_COPYABLE_(Queue);

    template <class U>
    void Push(U &&value) {
        std::lock_guard<FastLock> lock(lock_);
        queue_.push(std::forward<U>(value));
    }

    [[nodiscard]] bool Pop(Data &value) {
        std::lock_guard<FastLock> lock(lock_);

        if(queue_.empty()) {
            return false;
        }

        value = queue_.front();
        queue_.pop();
        return true;
    }

    Data Pop() {
        std::lock_guard<FastLock> lock(lock_);
        if(this->queue_.empty()) {
            return nullptr;
        }

        Data v = this->queue_.front();
        queue_.pop();
        return v;
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

    std::queue<Data> Clone() {
        std::lock_guard<FastLock> lock(lock_);

        std::queue<Data> q = this->queue_;
        return q;
    }

    void Clear() {
        std::lock_guard<FastLock> lock(lock_);
        std::queue<Data>          t;
        this->queue_.swap(t);
    }

private:
    std::queue<Data> queue_;
    FastLock         lock_;
};

template <class T>
class Queue<std::unique_ptr<T>> {
    using Data = std::unique_ptr<T>;

public:
    Queue()  = default;
    ~Queue() = default;

    NON_COPYABLE_(Queue);

    template <class U>
    void Push(U &&value) {
        std::lock_guard<FastLock> lock(lock_);
        queue_.push(std::forward<U>(value));
    }

    [[nodiscard]] bool Pop(Data &value) {
        std::lock_guard<FastLock> lock(lock_);

        if(queue_.empty()) {
            return false;
        }

        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    Data Pop() {
        std::lock_guard<FastLock> lock(lock_);
        if(this->queue_.empty()) {
            return nullptr;
        }

        Data v = std::move(this->queue_.front());
        queue_.pop();
        return v;
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

    std::queue<Data> Clone() {
        std::lock_guard<FastLock> lock(lock_);

        std::queue<Data> q = this->queue_;
        return q;
    }

    void Clear() {
        std::lock_guard<FastLock> lock(lock_);
        std::queue<Data>          t;
        this->queue_.swap(t);
    }

private:
    std::queue<Data> queue_;
    FastLock         lock_;
};

} // namespace lbox::thread_safe

#endif // LAZYBOX_INCLUDE_LAZYBOX_THREAD_THREAD_SAFE_QUEUE_HPP_
