#pragma once
#ifndef LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_
#define LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_

#include <cassert>

#include "lazybox/thread/Lock.hpp"
#include "NonCopyAble.hpp"

namespace lbox {

/**
 * 单例模板，懒汉式
 * @tparam T 单例类
 */
template <class T>
class Instance : public NonCopyAble {
public:
    using ValueType    = T;
    using ValuePointer = T *;

public:
    /**
     * thread safety
     * @return
     */
    static ValuePointer GetInstance() {
        static ValueType instance;
        return std::addressof(instance);
    }

protected:
    Instance()  = default;
    ~Instance() = default;
};

/**
 * 单例模板，懒汉式
 * @tparam T 单例类
 */
template <class T>
class GlobalInstance : public NonCopyAble {
public:
    using ValueType    = T;
    using ValuePointer = T *;

public:
    template <class... Types>
    static ValuePointer Init(Types &&...Args) {
        auto temp_p = instance_.load(std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_acquire); // 内存屏障

        if(temp_p == nullptr) {
            std::lock_guard lock{lock_};
            temp_p = instance_.load(std::memory_order_relaxed);

            if(temp_p == nullptr) {
                temp_p = new ValueType(std::forward<Types>(Args)...);
                std::atomic_thread_fence(std::memory_order_release); // 内存屏障
                instance_.store(temp_p, std::memory_order_relaxed);
            }
        }

        return temp_p;
    }

    static void Destroy() {
        delete GlobalInstance::instance_;
        GlobalInstance::instance_ = nullptr;
    }

    /**
     * thread safety
     * @return
     */
    static ValuePointer GetInstance() {
        auto instance = instance_.load(std::memory_order_relaxed);

        assert(instance && "instance has not initialized");
        return instance;
    }

private:
    static std::atomic<ValuePointer> instance_;
    static FastLock                  lock_;

protected:
    GlobalInstance()  = default;
    ~GlobalInstance() = default;
};

template <class T>
inline std::atomic<typename GlobalInstance<T>::ValuePointer> GlobalInstance<T>::instance_ = nullptr;

template <class T>
inline FastLock GlobalInstance<T>::lock_;

/**
 * 线程单例模式
 * @tparam T
 */
template <class T>
class ThreadInstance : public NonCopyAble {
public:
    using ValueType    = T;
    using ValuePointer = T *;

public:
    template <class... Types>
    static ValuePointer Init(Types &&...Args) {
        if(instance_ == nullptr) {
            instance_ = new ValueType{std::forward<Types>(Args)...};
        }

        return instance_;
    }

    static ValuePointer GetInstance() { return instance_; }

    static void Destroy() {
        delete instance_;
        instance_ = nullptr;
    }

private:
    static thread_local ValuePointer instance_;

protected:
    ThreadInstance()  = default;
    ~ThreadInstance() = default;
};

template <class T>
inline thread_local typename ThreadInstance<T>::ValuePointer ThreadInstance<T>::instance_ = nullptr;

} // namespace lbox

#endif // LAZYBOX_INCLUDE_TOY_INSTANCE_HPP_
