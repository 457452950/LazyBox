#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_
#define LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_

#include <memory>
#include <queue>

#include "lazybox/Assert.hpp"
#include "Lock.hpp"
#include "lazybox/base/TypeTraits.hpp"
#include "lazybox/thread_safe/Queue.hpp"

namespace lbox {


template <class T>
class Channel {
public:
    using value_type = T;
    using value_ref  = T &;

public:
    Channel() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~Channel() = default;

    NON_COPYABLE_(Channel);

    /**
     * non-blocking
     *
     * @tparam Uchanne
     * @param data
     */
    template <class U>
    void Push(U &&data) {
        this->data_que_.Push(std::forward<U>(data));
    }

    [[nodiscard]] bool Get(value_ref v) { return this->data_que_.Pop(v); }
    value_type         Get() { return this->data_que_.Pop(); }

    std::size_t Size() { return this->data_que_.Size(); }

    [[nodiscard]] bool Empty() { return this->data_que_.Empty(); }

    void Clear() { this->data_que_.Clear(); }

private:
    thread_safe::Queue<value_type> data_que_;
};


// waitable channel
template <class T, int max_size = INT16_MAX>
class WaitableChannel : protected Channel<T> {
public:
    using value_type = T;
    using value_ref  = T &;

public:
    WaitableChannel()  = default;
    ~WaitableChannel() = default;

    /**
     * blocked, push data to channel
     * block until data is pushed
     * @tparam U
     * @param data
     */
    template <class U>
    void Push(U &&data) {
        if(!this->active_.load(std::memory_order_relaxed)) {
            return;
        }

        {
            std::unique_lock un(this->lock_);

            // 防止虚假唤醒
            while(active_.load(std::memory_order_relaxed) && Channel<T>::Size() >= max_size) {

                control_ca_.wait(un, [this]() -> bool {
                    if(!this->active_.load(std::memory_order_relaxed)) {
                        return true;
                    }

                    if(Channel<T>::Size() < max_size) {
                        return true;
                    }

                    return false;
                });
            }

            if(!active_.load(std::memory_order_relaxed)) {
                return;
            }

            Channel<T>::Push(std::forward<U>(data));
        }
        this->WakeUpAll();
    }

    void Get(value_ref v) {
        if(!active_.load(std::memory_order_relaxed)) {
            return;
        }
        {
            std::unique_lock un(this->lock_);

            // 防止虚假唤醒
            while(active_.load(std::memory_order_relaxed) && Channel<T>::Empty()) {

                this->control_ca_.wait(un, [this]() -> bool {
                    if(!active_.load(std::memory_order_relaxed))
                        return true;

                    return !Channel<T>::Empty();
                });
            }

            if(!active_.load(std::memory_order_relaxed)) {
                return;
            }

            Assert(Channel<T>::Get(v) == true, "waitable channel get value false");
        }
        this->WakeUpAll();
    }

    std::size_t Size() {
        std::unique_lock un(this->lock_);
        return Channel<T>::Size();
    }

    bool Empty() {
        std::unique_lock un(this->lock_);
        return Channel<T>::Empty();
    }

    void Clear() {
        std::unique_lock un(this->lock_);
        Channel<T>::Clear();
    }

    void Close() {
        active_.store(false, std::memory_order_seq_cst);
        this->WakeUpAll();
    }

protected:
    void WakeUpAll() { this->control_ca_.notify_all(); }
    void WakeUp() { this->control_ca_.notify_one(); }

private:
    std::mutex              lock_;
    std::condition_variable control_ca_;
    std::atomic_bool        active_{true};
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_
