#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_
#define LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_

#include <memory>
#include <queue>

#include "Lock.hpp"
#include "base/TypeTraits.hpp"

namespace lbox {
/**
 * Actor模型
 * @tparam T
 */
template <class T>
class Actor {
    using value_type = T;

public:
    virtual ~Actor() = default;

    virtual void Send(value_type m) {
        UniqueLock unl(lock_);
        msg_que_.push(m);
    }

protected:
    std::size_t Size() { return msg_que_.size(); }

    value_type Get() noexcept(false) {
        UniqueLock unl(lock_);
        auto       res = msg_que_.front();
        msg_que_.pop();
        return res;
    }

private:
    FastLock               lock_;
    std::queue<value_type> msg_que_;
};

template <class T>
    requires(std::is_pointer_v<T> || is_template_of_v<std::shared_ptr, T>)
class Actor<T> {
    using value_type = T;

public:
    virtual ~Actor() = default;

    virtual void Send(value_type m) {
        UniqueLock unl(lock_);
        msg_que_.push(m);
    }

protected:
    std::size_t Size() {
        UniqueLock unl(lock_);
        return msg_que_.size();
    }

    value_type Get() {
        UniqueLock unl(lock_);
        if(this->msg_que_.empty()) {
            return nullptr;
        }

        auto res = msg_que_.front();
        msg_que_.pop();
        return res;
    }

private:
    FastLock               lock_;
    std::queue<value_type> msg_que_;
};


template <class T>
class BActor {
    using value_type = T;

public:
    virtual ~BActor() = default;

    virtual void Send(value_type m) {
        {
            std::unique_lock uni(this->control_mutex_);
            msg_que_.push(m);
        }
        this->WakeUp();
    }

protected:
    std::size_t Size() {
        std::unique_lock uni(this->control_mutex_);
        return msg_que_.size();
    }

    value_type Get() {
        std::unique_lock un(this->lock_);

        this->control_ca_.wait(un);

        if(this->msg_que_.empty()) {
            return value_type{};
        }

        value_type ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
    }

    void WakeUp() { this->control_ca_.notify_one(); }

private:
    std::queue<value_type> msg_que_;

    std::mutex              control_mutex_;
    std::condition_variable control_ca_;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_
