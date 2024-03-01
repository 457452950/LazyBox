#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_
#define LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_

#include <memory>
#include <queue>

#include "Lock.hpp"
#include "lazybox/base/TypeTraits.hpp"
#include "lazybox/thread_safe/Queue.hpp"

namespace lbox {


template <class T>
class Channel : public NonCopyAble {
public:
    using value_type = T;
    using value_ref  = T &;

public:
    Channel() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~Channel() = default;

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

    [[nodiscard]] bool Get(value_ref v) noexcept(true) { return this->data_que_.Pop(v); }

    std::size_t Size() { return this->data_que_.Size(); }

    [[nodiscard]] bool Empty() { return this->data_que_.Empty(); }

    void Clear() { this->data_que_.Clear(); }

private:
    thread_safe::Queue<value_type> data_que_;
};


template <class T>
    requires(std::is_pointer_v<T> || is_template_of_v<std::shared_ptr, T>)
class Channel<T> : public NonCopyAble {
public:
    using value_type = T;

public:
    Channel() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~Channel() = default;

    template <class U>
    void Push(U &&data) {
        std::lock_guard uni{lock_};
        this->data_que_.push(std::forward<U>(data));
    }

    [[nodiscard]] value_type Get() {
        if(this->data_que_.empty()) {
            return nullptr;
        }

        std::lock_guard uni{lock_};
        value_type      ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
    }

    [[nodiscard]] std::size_t Size() {
        std::lock_guard uni{lock_};
        return this->data_que_.size();
    }

    [[nodiscard]] bool Empty() {
        std::lock_guard uni{lock_};
        return this->data_que_.empty();
    }

    void Clear() {
        std::lock_guard        uni{lock_};
        std::queue<value_type> empty;
        this->data_que_.swap(empty);
    }

private:
    std::queue<value_type> data_que_;
    FastLock               lock_;
};

// waitable channel
template <class T, int max_size = INT16_MAX>
class WaitableChannel : public NonCopyAble {
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
            while(active_.load(std::memory_order_relaxed) && this->data_que_.size() >= max_size) {

                control_ca_.wait(un, [this]() -> bool {
                    if(!this->active_.load(std::memory_order_relaxed)) {
                        return true;
                    }

                    if(this->data_que_.size() < max_size) {
                        return true;
                    }

                    return false;
                });
            }

            if(!active_.load(std::memory_order_relaxed)) {
                return;
            }

            this->data_que_.push(std::forward<U>(data));
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
            while(active_.load(std::memory_order_relaxed) && this->data_que_.empty()) {

                this->control_ca_.wait(un, [this]() -> bool {
                    if(!active_.load(std::memory_order_relaxed))
                        return true;

                    return !this->data_que_.empty();
                });
            }

            if(!active_.load(std::memory_order_relaxed)) {
                return;
            }

            v = std::move(this->data_que_.front());
            this->data_que_.pop();
        }
        this->WakeUpAll();
    }

    std::size_t Size() {
        std::unique_lock un(this->lock_);
        return this->data_que_.size();
    }

    bool Empty() {
        std::unique_lock un(this->lock_);
        return this->data_que_.empty();
    }

    void Clear() {
        std::unique_lock       un(this->lock_);
        std::queue<value_type> empty;
        this->data_que_.swap(empty);
    }

    void Close() {
        active_.store(false, std::memory_order_acquire);
        this->WakeUpAll();
        this->Clear();
    }

protected:
    void WakeUpAll() { this->control_ca_.notify_all(); }
    void WakeUp() { this->control_ca_.notify_one(); }

private:
    std::queue<value_type> data_que_;

    std::mutex              lock_;
    std::condition_variable control_ca_;
    std::atomic_bool        active_{true};
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_
