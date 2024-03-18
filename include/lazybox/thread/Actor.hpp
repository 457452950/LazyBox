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
protected:
    using value_type = T;
    using value_ref  = T &;
    using value_ptr  = T *;

public:
    Actor()          = default;
    virtual ~Actor() = default;

    /**
     * non-blocking
     * @param m
     */
    template <class U>
    void Send(U &&m) {
        if(this->SendingMessageFilter(m)) {
            msg_que_.Push(std::forward<U>(m));
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
    std::size_t Size() noexcept(true) { return msg_que_.Size(); }

    [[nodiscard]] bool Empty() noexcept(true) { return msg_que_.Empty(); }

    /**
     * non-blocking
     * @param v
     * @return 返回true时有消息，返回false时无消息
     */
    [[nodiscard]] bool Get(value_ref v) noexcept(true) { return this->msg_que_.Pop(v); }

    value_type Get() noexcept(false) { return this->msg_que_.Pop(); }

    void Clear() { this->msg_que_.Clear(); }

private:
    thread_safe::Queue<value_type> msg_que_;
};

/**
 * 可阻塞Actor
 * @tparam T
 */
template <class T>
class WaitableActor : public Actor<T> {
protected:
    using value_type = typename Actor<T>::value_type;
    using value_ref  = typename Actor<T>::value_ref;

public:
    WaitableActor() = default;
    virtual ~WaitableActor(){
            //        this->Clear();
    };

    /**
     * non-blocking
     * @param m
     */
    template <class U>
    void Send(U &&m) {
        Actor<T>::Send(std::forward<U>(m));
        this->WakeUp();
    }


    /**
     * 数据过滤器
     * @param v 数据
     * @return 返回false时略过此条数据
     */
    [[nodiscard]] virtual bool SendingMessageFilter(const value_type & /* v */) { return true; }

protected:
    /**
     * blocking
     * @param v
     * @return false for empty,
     *         true for success
     */
    [[nodiscard]] bool Wait2Get(value_ref v) {
        {
            std::unique_lock un(this->control_mutex_);

            if(this->Empty()) {
                this->control_ca_.wait(un);
            }
        }

        return Actor<T>::Get(v);
    }

    value_type Wait2Get() {
        {
            std::unique_lock un(this->control_mutex_);

            if(this->Empty()) {
                this->control_ca_.wait(un);
            }
        }

        return Actor<T>::Get();
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
    [[nodiscard]] bool Wait2Get(value_ref v, const std::chrono::duration<Rep, Period> &rel_time) {
        {
            std::unique_lock un(this->control_mutex_);

            if(this->Empty()) {
                this->control_ca_.wait_for(un, rel_time);
            }
        }

        return Actor<T>::Get(v);
    }

    template <class Rep, class Period>
    value_type Wait2Get(const std::chrono::duration<Rep, Period> &rel_time) {
        {
            std::unique_lock un(this->control_mutex_);

            if(this->Empty()) {
                this->control_ca_.wait_for(un, rel_time);
            }
        }

        return Actor<T>::Get();
    }


    /**
     * wake up the actor
     */
    void WakeUp() { this->control_ca_.notify_one(); }

private:
    std::mutex              control_mutex_;
    std::condition_variable control_ca_;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_ACTOR_HPP_
