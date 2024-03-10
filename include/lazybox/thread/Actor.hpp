#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_
#define LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_

#include <memory>
#include <queue>

#include "Lock.hpp"
#include "lazybox/base/TypeTraits.hpp"
#include "lazybox/thread_safe/Queue.hpp"

namespace lbox {

/**
 * Actor模型
 * 多生产者，单一消费者
 * @tparam T
 */
template <class T>
class Actor : public NonCopyAble {
    using value_type = T;
    using value_ref  = T &;
    using value_ptr  = T *;

public:
    virtual ~Actor() = default;

    /**
     * non-blocking
     * @param m
     */
    template <class U>
    void Send(U &&m) {
        if(this->SendingMessageFilter(m)) {
            std::lock_guard uni{lock_};
            msg_que_.push(std::forward<U>(m));
        }
    }

protected:
    /**
     * 数据过滤器
     * @param v 数据
     * @return 返回false时略过此条数据
     */
    virtual bool SendingMessageFilter(const value_type & /* v */) { return true; }

protected:
    std::size_t Size() noexcept(true) {
        std::lock_guard uni{lock_};
        return msg_que_.size();
    }

    [[nodiscard]] bool Empty() noexcept(true) {
        std::lock_guard uni{lock_};
        return msg_que_.empty();
    }

    /**
     * non-blocking
     * @param v
     * @return 返回true时有消息，返回false时无消息
     */
    [[nodiscard]] bool Get(value_ref v) noexcept(true) {
        if(Empty()) {
            return false;
        }

        std::lock_guard uni{lock_};

        v = std::move(msg_que_.front());
        msg_que_.pop();
        return true;
    }

    void Clear() noexcept(true) {
        std::lock_guard        uni{lock_};
        std::queue<value_type> empty;
        this->msg_que_.swap(empty);
    }

private:
    FastLock               lock_;
    std::queue<value_type> msg_que_;
};

/**
 * Actor模式指针版本
 * @tparam T
 */
template <class T>
    requires(std::is_pointer_v<T> || is_template_of_v<std::shared_ptr, T>)
class Actor<T> : public NonCopyAble {
    using value_type = T;

public:
    virtual ~Actor() = default;

    /**
     * non-blocking
     * @param m
     */
    template <class U>
    void Send(U &&m) {
        if(this->SendingMessageFilter(m)) {
            std::lock_guard uni{lock_};
            msg_que_.push(std::forward<U>(m));
        }
    }

    /**
     * 数据过滤器
     * @param v 数据
     * @return 返回false时略过此条数据
     */
    [[nodiscard]] virtual bool SendingMessageFilter(const value_type & /* v */) { return true; }

protected:
    std::size_t Size() {
        std::lock_guard uni{lock_};
        return msg_que_.size();
    }

    [[nodiscard]] bool Empty() noexcept(true) {
        std::lock_guard uni{lock_};
        return msg_que_.empty();
    }

    /**
     * non-blocking
     * @return nullptr for empty,
     *         otherwise return the data
     */
    [[nodiscard]] value_type Get() noexcept(true) {
        std::lock_guard uni{lock_};

        if(this->msg_que_.empty()) {
            return nullptr;
        }

        auto res = std::move(msg_que_.front());
        msg_que_.pop();
        return res;
    }

    void Clear() noexcept(true) {
        std::lock_guard        uni{lock_};
        std::queue<value_type> empty;
        this->msg_que_.swap(empty);
    }

private:
    FastLock               lock_;
    std::queue<value_type> msg_que_;
};

/**
 * 可阻塞Actor
 * @tparam T
 */
template <class T>
class WaitableActor : public NonCopyAble {
    using value_type = T;
    using value_ref  = T &;

public:
    virtual ~WaitableActor() = default;

    /**
     * non-blocking
     * @param m
     */
    template <class U>
    void Send(U &&m) {
        if(this->SendingMessageFilter(m)) {
            msg_que_.Push(std::forward<U>(m));
            this->WakeUp();
        }
    }


    /**
     * 数据过滤器
     * @param v 数据
     * @return 返回false时略过此条数据
     */
    [[nodiscard]] virtual bool SendingMessageFilter(const value_type & /* v */) { return true; }

protected:
    std::size_t Size() noexcept(true) {
        std::unique_lock uni(this->control_mutex_);
        //        return msg_que_.size();
        return msg_que_.Size();
    }

    bool Empty() noexcept(true) {
        std::unique_lock uni(this->control_mutex_);
        //        return msg_que_.empty();
        return msg_que_.Empty();
    }

    /**
     * blocking
     * @param v
     * @return false for empty,
     *         true for success
     */
    [[nodiscard]] bool Get(value_ref v) {
        {
            std::unique_lock un(this->control_mutex_);

            if(this->msg_que_.Empty()) {
                this->control_ca_.wait(un);
            }
        }

        if(this->msg_que_.Pop(v)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * blocking with timeout
     * @tparam Rep
     * @tparam Period
     * @param v
     * @param rel_time
     * @return
     */
    template <class Rep, class Period>
    [[nodiscard]] bool Get(value_ref v, const std::chrono::duration<Rep, Period> &rel_time) {
        {
            std::unique_lock un(this->control_mutex_);

            if(this->msg_que_.Empty()) {
                this->control_ca_.wait_for(un, rel_time);
            }
        }

        if(this->msg_que_.Pop(v)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * non-blocking
     * @param v
     * @return 返回true时有消息，返回false时无消息
     */
    [[nodiscard]] bool TryGet(value_ref v) noexcept(true) {
        //        if(Empty()) {
        //            return false;
        //        }
        //
        //        std::lock_guard uni{control_mutex_};
        //
        //        v = std::move(msg_que_.front());
        //        msg_que_.pop();
        //        return true;

        if(this->msg_que_.Pop(v)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * wake up the actor
     */
    void WakeUp() { this->control_ca_.notify_one(); }

    void Clear() noexcept(true) {
        std::lock_guard uni{control_mutex_};
        //        std::queue<value_type> empty;
        //        this->msg_que_.swap(empty);

        msg_que_.Clear();
    }

private:
    //    std::queue<value_type> msg_que_;
    typename thread_safe::Queue<value_type> msg_que_;

    std::mutex              control_mutex_;
    std::condition_variable control_ca_;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_
