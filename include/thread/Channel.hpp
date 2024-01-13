#pragma once
#ifndef LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_
#define LAZYBOX_INCLUDE_THREAD_MESSAGE_QUEUE_H_

#include <memory>
#include <queue>

#include "Lock.hpp"
#include "base/TypeTraits.hpp"

namespace lbox {


template <class T>
class Channel {
public:
    using value_type = T;

    // no copy able
    Channel &operator=(const Channel &) = delete;
    Channel(const Channel &)            = delete;

public:
    Channel() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~Channel() = default;

    template <class U>
    void Push(U &&data) {
        UniqueLock uni(lock_);
        this->data_que_.push(std::forward<T>(data));
    }

    value_type Get() noexcept(false) {
        UniqueLock uni(lock_);
        value_type ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
    }

    std::size_t Size() {
        UniqueLock uni(lock_);
        return this->data_que_.size();
    }

    bool Empty() {
        UniqueLock uni(lock_);
        return this->data_que_.empty();
    }

    void Clear() {
        UniqueLock uni(lock_);
        while(!this->data_que_.empty())
            this->data_que_.pop();
    }

private:
    std::queue<value_type> data_que_;
    FastLock               lock_;
};


template <class T>
    requires(std::is_pointer_v<T> || is_template_of_v<std::shared_ptr, T>)
class Channel<T> {
public:
    using value_type = T;

    // no copy able
    Channel &operator=(const Channel &) = delete;
    Channel(const Channel &)            = delete;

public:
    Channel() { static_assert(!std::is_reference_v<T> && !std::is_array_v<T> && !std::is_const_v<T>); }
    ~Channel() = default;

    template <class U>
    void Push(U &&data) {
        UniqueLock uni(lock_);
        this->data_que_.push(std::forward<T>(data));
    }

    value_type Get() {
        UniqueLock uni(lock_);
        if(this->data_que_.empty()) {
            return nullptr;
        }
        value_type ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
    }

    std::size_t Size() {
        UniqueLock uni(lock_);
        return this->data_que_.size();
    }

    bool Empty() {
        UniqueLock uni(lock_);
        return this->data_que_.empty();
    }

    void Clear() {
        UniqueLock uni(lock_);
        while(!this->data_que_.empty())
            this->data_que_.pop();
    }

private:
    std::queue<value_type> data_que_;
    FastLock               lock_;
};

// blocked channel
template <class T>
class BChannel {
public:
    using value_type = T;

    // no copy able
    BChannel &operator=(const BChannel &) = delete;
    BChannel(const BChannel &)            = delete;

public:
    BChannel()  = default;
    ~BChannel() = default;

    template <class U>
    void Push(U &&data) {
        if(this->active_) {
            return;
        }

        {
            std::unique_lock un(this->lock_);
            this->data_que_.push(std::forward<T>(data));
        }
        this->WakeUpAll();
    }

    value_type Get() {
        if(!active_) {
            return value_type{};
        }

        std::unique_lock un(this->lock_);

        this->control_ca_.wait(un, [this]() -> bool {
            if(!active_)
                return true;

            return !this->Empty();
        });

        if(!active_) {
            return value_type{};
        }

        value_type ret = std::move(this->data_que_.front());
        this->data_que_.pop();
        return ret;
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
        std::unique_lock un(this->lock_);
        while(!this->data_que_.empty())
            this->data_que_.pop();
    }

    void Close() {
        active_.store(false);
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
