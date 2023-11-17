#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_MESSAGEQUEUE_H_
#define LAZYBOX_INCLUDE_THREAD_MESSAGEQUEUE_H_

#include <memory>
#include <atomic>
#include <queue>
#include <stdexcept>

#include "base/TypeTraits.h"

namespace lbox {

class UniqueFlag {
public:
    explicit UniqueFlag(std::atomic_flag &flag) : flag_(flag) {
        while(flag_.test_and_set()) {
        }
    }
    ~UniqueFlag() { flag_.clear(); }

private:
    std::atomic_flag &flag_;
};

template <class T>
class MQueue {
public:
    using value_type = T;

public:
    MQueue() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~MQueue() = default;

    template <class U>
    void Push(U &&data) {
        UniqueFlag uni(control_flag_);
        this->data_que_.push(std::forward<T>(data));
    }

    value_type Get() noexcept(false) {
        UniqueFlag uni(control_flag_);
        if(this->data_que_.empty()) {
            throw std::out_of_range{"message queue empty."};
        }

        value_type ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
    }

    std::size_t Size() {
        UniqueFlag uni(control_flag_);
        return this->data_que_.size();
    }

    bool Empty() {
        UniqueFlag uni(control_flag_);
        return this->data_que_.empty();
    }

    void Clear() {
        UniqueFlag uni(control_flag_);
        while(!this->data_que_.empty())
            this->data_que_.pop();
    }

private:
    std::queue<value_type> data_que_;
    std::atomic_flag       control_flag_;
};


template <class T>
    requires(std::is_pointer_v<T> || is_template_of_v<std::shared_ptr, T>)
class MQueue<T> {
public:
    using value_type = T;

public:
    MQueue() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~MQueue() = default;

    template <class U>
    void Push(U &&data) {
        UniqueFlag uni(control_flag_);
        this->data_que_.push(std::forward<T>(data));
    }

    value_type Get() {
        UniqueFlag uni(control_flag_);
        if(this->data_que_.empty()) {
            return nullptr;
        }
        value_type ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
    }

    std::size_t Size() {
        UniqueFlag uni(control_flag_);
        return this->data_que_.size();
    }

    bool Empty() {
        UniqueFlag uni(control_flag_);
        return this->data_que_.empty();
    }

    void Clear() {
        UniqueFlag uni(control_flag_);
        while(!this->data_que_.empty())
            this->data_que_.pop();
    }

private:
    std::queue<value_type> data_que_;
    std::atomic_flag       control_flag_;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_MESSAGEQUEUE_H_
